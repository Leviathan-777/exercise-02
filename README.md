# exercise-02
Solution to Exercise-02
The program is doing the following tasks:

Parse the data from data/data.json
Discard the devices where the timestamp value is before the current time. The timestamps are in UNIX format
Get the total of all value entries, values are base64 strings containing integer values
Parse the uuid from the info field of each entry
Output the values total and the list of uuids in the format described by the JSON schema. Writes it to output_data.json file

#How to run it
The program requires
The program can be run on Linux by following these steps:


Alternativly it can be run in the docker container by using these commands in the terminal:
1. docker build -t exercise02-solution .
2. docker run -it exercise02-solution
3. ./exercise02_solution
Then the program should write data to output_data.json inside the container.
