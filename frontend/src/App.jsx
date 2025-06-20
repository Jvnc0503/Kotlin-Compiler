// src/App.jsx
import React, { useState } from "react";
import Editor from "@monaco-editor/react";
import hljs from "highlight.js/lib/core";
import x86asm from "highlight.js/lib/languages/x86asm";
import axios from "axios";
import "./App.css";

hljs.registerLanguage("x86asm", x86asm);

// Definir la URL de la API en un sólo lugar
const apiUrl = import.meta.env.VITE_API_URL;

export default function App() {
  const [code, setCode] = useState(
    "fun main() : Int {\n    var i: Int = 0\n    while (i <= 5) {\n        println(i)\n        i = i + 1\n    }\n}"
  );
  const [stdout, setOut] = useState("");
  const [assembly, setAsm] = useState("");

  const run = async () => {
    const payload = { source: code };

    console.log("payload:", JSON.stringify(payload));

    try {
      const { data } = await axios.post(apiUrl, JSON.stringify(payload), {
        headers: { "Content-Type": "application/json" },
      });
      setOut(data.result ?? data.error ?? "");
      setAsm(data.asm ?? "");
    } catch (err) {
      const message =
        err.response?.data?.error || err.message || "Unknown error";
      setOut(String(message));
      setAsm("");
    }
  };

  return (
    <div className="container">
      <div className="editor-panel">
        <button className="run-btn" onClick={run}>
          ▶ Run
        </button>
        <div className="editor-wrapper">
          <Editor
            height="100%"
            defaultLanguage="kotlin"
            value={code}
            onChange={(v) => setCode(v ?? "")}
            theme="vs-dark"
            options={{
              padding: { top: 12 },
              minimap: { enabled: false },
            }}
          />
        </div>
      </div>

      <div className="output-panel">
        <section>
          <h3>Program output</h3>
          <pre className="output">{stdout || "—"}</pre>
        </section>
        <section>
          <h3>Assembly</h3>
          <pre
            className="asm"
            dangerouslySetInnerHTML={{
              __html: assembly
                ? hljs.highlight(assembly, { language: "x86asm" }).value
                : "",
            }}
          />
        </section>
      </div>
    </div>
  );
}
