#include <stdio.h>
#include <jansson.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "decode_base64.c"

typedef struct {
    char** uuids;
    int num_of_uuids;
    int valueTotal;
} ParsedData;

// Function compares given unix timestamp with current unix timestamp
bool checkTimestamp(const time_t timestamp)
{
    //Take the timestamp of current time
    time_t current_time = time(NULL);

    if (timestamp > current_time)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Decodes base64 value and convert it to int
int base64_decode_to_int(const char *data) {
    size_t output_length;
    unsigned char *decoded_data = base64_decode(data, strlen(data), &output_length);
    int value = 0;

    if (decoded_data != NULL) {
        value = atoi(decoded_data);
        free(decoded_data);
    }

    return value;
}

//Saves totalValue and uuids data to json file
int save_data_to_json(const char** ids, int numIds, int valueTotal, const char* filename) {
    // Create the JSON objects
    json_t *root, *uuids, *value;
    root = json_object();
    uuids = json_array();
    value = json_integer(valueTotal);

    // Add the IDs to the UUIDS array
    for (int i = 0; i < numIds; i++) {
        json_array_append_new(uuids, json_string(ids[i]));
    }

    // Add the UUIDS array and valueTotal to the root object
    json_object_set_new(root, "ValueTotal", value);
    json_object_set_new(root, "UUIDS", uuids);

    // Save the JSON object to a file
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }

    json_dumpf(root, file, JSON_INDENT(2));
    fclose(file);
    printf("\nJSON File saved successfuly as %s\n",filename);
    // Remove root value
    json_decref(root);
    return 0;
}


// Function opens and reads the contents of the json file
ParsedData parse_json_file(const char *filename)
{
    ParsedData parsedData;
    parsedData.uuids = NULL;
    parsedData.valueTotal = 0;
    // Open and read the JSON file
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Failed to open JSON file\n");
        return parsedData;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer)
    {
        printf("Failed to allocate memory\n");
        fclose(file);
        return parsedData;
    }
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    // Parse the JSON data
    json_error_t error;
    json_t *root = json_loads(buffer, 0, &error);
    free(buffer);
    if (!root)
    {
        printf("Failed to parse JSON: %s\n", error.text);
        return parsedData;
    }

    // Access the JSON data
    json_t *devices_array = json_object_get(root, "Devices");
    size_t devices_count = json_array_size(devices_array);
    
    int valueTotal = 0;
    int index = 0;
    char* uuids[devices_count];
    
    for (int i = 0; i < devices_count; i++) {
        uuids[i] = NULL;
    }

    for (size_t i = 0; i < devices_count; i++)
    {
        json_t *device = json_array_get(devices_array, i);

        json_t *timestamp = json_object_get(device, "timestamp");
        const char *timestamp_value = json_string_value(timestamp);

        time_t device_time = (time_t)atoi(timestamp_value);
        bool time_check = checkTimestamp(device_time);
        if (time_check == true)
        {
            json_t *info = json_object_get(device, "Info");
            const char *info_value = json_string_value(info);

            const char *uuid_start = strstr(info_value, "uuid:");
            if (uuid_start)
            {
                uuid_start += strlen("uuid:");
                const char *uuid_end = strchr(uuid_start, ',');
                if (uuid_end)
                {
                    size_t uuid_length = uuid_end - uuid_start;
                    char uuid[uuid_length + 1];
                    strncpy(uuid, uuid_start, uuid_length);
                    uuid[uuid_length] = '\0';
                    uuids[index] = (char*)malloc((strlen(uuid) + 1) * sizeof(char));
                    strcpy(uuids[index], uuid);
                    index += 1;

                    json_t *value = json_object_get(device, "value");
                    const char *value_value = json_string_value(value);
                    int decoded_value = base64_decode_to_int(value_value);
                    valueTotal += decoded_value;

                    printf("Device %zu:\n", i + 1);
                    printf("UUID: %s\n", uuid);
                    printf("Decoded Value: %d\n", decoded_value);
                    printf("\n");
                }
            }
        }
    }
    printf("Value Total: %d\n", valueTotal);
    parsedData.uuids = uuids;
    parsedData.valueTotal = valueTotal;
    parsedData.num_of_uuids = index;
    //const char *output_filename = "output_data.json";
    //save_data_to_json(uuids, index, valueTotal, output_filename);
    
    json_decref(root);
    return parsedData;
}

int main()
{
    const char *filename = "../data/data.json";
    ParsedData results = parse_json_file(filename);
    if(results.num_of_uuids>0){
    const char *output_filename = "output_data.json";
    save_data_to_json(results.uuids, results.num_of_uuids, results.valueTotal, output_filename);
    }
    else{
        printf("No valid devices. The data was not saved.");
    }
    return 0;
}