# exercise-02
Solution to Exercise-02
The program is doing the following tasks:

Parse the data from data/data.json
Discard the devices where the timestamp value is before the current time. The timestamps are in UNIX format
Get the total of all value entries, values are base64 strings containing integer values
Parse the uuid from the info field of each entry
Output the values total and the list of uuids in the format described by the JSON schema. Writes it to output_data.json file

#How to run it
The program requires Jannson library which is used for Parsing JSON files.
The program can be run on Linux by running these commands in terminal from exercises directory:
1. apt-get update
2. apt-get install -y libjansson-dev
3. gcc -o exercise02_solution src/exercise02_solution.c -ljansson
4. ./exercise02_solution
Then the program should write data to output_data.json.


Alternativly it can be run in the docker container by using these commands in terminal from exercises directory:
1. docker build -t exercise02-solution .
2. docker run -it exercise02-solution
3. ./exercise02_solution
Then the program should write data to output_data.json inside the container.
