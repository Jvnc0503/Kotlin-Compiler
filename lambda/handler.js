// handler.js  – copia y pega todo el archivo

// ---------- dependencias ----------
const fs        = require('fs').promises;
const os        = require('os');
const path      = require('path');
const { execFile } = require('child_process');
const util      = require('util');

const exec = util.promisify(execFile);

// ---------- rutas a tu tool-chain ----------
const X   = '/workspace/bin/x';      // tu compilador Kotlin-→ASM
const GCC = '/workspace/gcc/gcc';    // mini-gcc que acabas de empaquetar

// ---------- función Lambda ----------
exports.compile = async (event) => {
  // 1.  parsear el cuerpo
  const { source = '' } = JSON.parse(event.body || '{}');
  if (!source.trim())
    return { statusCode: 400, body: '"Empty source"' };

  // 2.  directorio temporal por invocación
  const work = await fs.mkdtemp(path.join(os.tmpdir(), 'kc-'));

  // 3.  guardar el código fuente
  await fs.writeFile(path.join(work, 'prog.kt'), source);


  try {
    // ----------  1) frontend: Kotlin → assembly ----------
    await exec(X, ['prog.kt'], { cwd: work, timeout: 10_000 });

    // ----------  2) backend: ensamblar + enlazar ----------
    await exec(
      GCC,
      [
        '-B', '/workspace/gcc',      // crt*.o  +  libgcc.a
        '-L', '/workspace/gcc',      // libgcc_s.so  +  libc.so
        '-fno-use-linker-plugin',    // evita liblto_plugin.so
        /*  Descomenta para binario 100 % estático
        '-static',
        */
        'output.s', '-o', 'a.out'
      ],
      { cwd: work, timeout: 15_000 }
    );

    //  (por si gcc no marcó el bit de ejecución)
    await fs.chmod(path.join(work, 'a.out'), 0o755);

    // ----------  3) ejecutar el binario ----------
    const { stdout } = await exec('./a.out', { cwd: work, timeout: 5_000 });

    const asm = await fs.readFile(path.join(work, 'output.s'), 'utf8');

    // éxito: devolver resultado al frontend
    return { statusCode: 200, body: JSON.stringify({ result: stdout, asm: asm }) };

  } catch (err) {
    // fallo en cualquier fase → devolver detalle
    console.error('exec error →', err);           // visible en CloudWatch
    return {
      statusCode: 400,
      body: JSON.stringify({
        exitCode: err.code,
        signal : err.signal,
        stderr : (err.stderr || '').toString()
      })
    };
  }
};
