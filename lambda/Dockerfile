FROM public.ecr.aws/lambda/nodejs:20

COPY bin  /workspace/bin
COPY gcc  /workspace/gcc
COPY handler.js .

RUN mkdir -p /usr/lib64 && \
    cp /workspace/gcc/libc_nonshared.a /usr/lib64/

ENV PATH="/workspace/gcc:${PATH}"
ENV LD_LIBRARY_PATH="/workspace/gcc"

CMD [ "handler.compile" ]
