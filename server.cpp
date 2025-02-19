//////////////////////////////////////////////////////////////////////////
// author: Digvijay Anand
// last modified: 02-18-2025
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <vector>

#define SHM_HASH_SIZE 4096  // 4KB
#define SHM_HASH_NAME "/shm_hash_table"

#define SHM_DATA_SIZE 4096*2  // 4KB
#define SHM_DATA_NAME "/shm_data_file"

using namespace std;

// void image_data_csv(const char* filename){

// }

void load_csv_data(const char* filename){

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening CSV file: " << filename << std::endl;
        return;
    }

    
    // Read Float Values from CSV and Store in Shared Memory
    std::vector<float> float_values;
    std::string line;
    if (std::getline(file, line)) {
        // std::cout << "Skipping first line " << std::endl;
    }
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        float value;
        while (ss >> value) {
            float_values.push_back(value);
            // cout << value << '\t' ;
            if (ss.peek() == ',') ss.ignore();  // Ignore commas
        }
    }
    // Copy float values directly into shared memory
    size_t float_data_size = float_values.size() * sizeof(float);
    cout << "size of float data from the file -- \t " << float_data_size << "\t " <<  float_values.size() << endl;
    if (float_data_size > SHM_DATA_SIZE) {
        std::cerr << "Error: Shared memory too small for data.\n";
        return;
    }
    else 
        cout << "size of float data from the file -- \t " << float_data_size << endl;

    // initialize a hash table to store the quantized values 

    // initialize a shared memory to hold the data from the csv file:
    int server_fd = shm_open(SHM_DATA_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(server_fd, SHM_DATA_SIZE);

    void* shared_memory = mmap(nullptr, SHM_DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, server_fd, 0);
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Failed to map shared memory.\n";
        return;
    }

    memcpy(shared_memory, float_values.data(), float_data_size);

    std::cout << "Float values from CSV loaded into shared memory.\n";

    // Cleanup
    file.close();
    munmap(shared_memory, SHM_DATA_SIZE);
    close(server_fd);

}

int main(int argc, char* argv[]){
    cout << "[ server ] Hello world" << endl;  

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <csv_filename>\n";
        return 1;
    }

    load_csv_data(argv[1]);
    


    
    // cout << "Data written to shared memory." << endl;
    // sleep(10);

    // // remove map
    // munmap(data, SHM_DATA_SIZE);
    
    // // remove the shared memory space
    // shm_unlink(SHM_DATA_NAME);


    // // mmap uasge:
    // void* virtual_page = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // if (virtual_page == MAP_FAILED) {
    // perror("mmap failed");
    //     return 1;
    // }
    // else 
    //     cout << "virtual page alllocation done -\t" << virtual_page << endl;

    // munmap(virtual_page, PAGE_SIZE);

    return 0;
}