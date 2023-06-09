FROM gcc:latest

WORKDIR /app

COPY src /app/src

COPY data/data.json /app/data/data.json

RUN apt-get update && \
    apt-get install -y libjansson-dev

RUN gcc -o exercise02_solution src/exercise02_solution.c -ljansson

CMD ["./exercise02_solution"]