//////////////////////////////////////////////////////////////////////////
// author: Digvijay Anand
// last modified: 02-18-2025
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <vector>

#define SHM_DATA_SIZE 4096
#define SHM_DATA_NAME "/shm_data_file"

using namespace std;

void* read_shared_memory(void* arg){
    int client_fd = shm_open(SHM_DATA_NAME, O_RDONLY, 0666);
    void* shared_memory = mmap(nullptr, SHM_DATA_SIZE, PROT_READ, MAP_SHARED, client_fd, 0);

    if (shared_memory == MAP_FAILED) {
        std::cerr << "Thread failed to attach to shared memory.\n";
        return nullptr;
    }

    float* float_data = static_cast<float*>(shared_memory);
    size_t num_floats = SHM_DATA_SIZE / sizeof(float);  // Calculate how many floats fit
    cout << "num_floats -- \t " << num_floats << endl;
    std::cout << "Thread " << pthread_self() << " read float values:\n";
    for (size_t i = 0; i < num_floats; i++) {
        std::cout << i << "\t"<< float_data[i] << "\n";
    }
    std::cout << std::endl;

    // Cleanup
    munmap(shared_memory, SHM_DATA_SIZE);
    close(client_fd);
    return nullptr;
}

int main(int argc, char* argv[]){
    cout << "[ client ] Hello world" << endl; 

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_threads>\n";
        return 1;
    }


    int num_threads = std::atoi(argv[1]);
    if (num_threads <= 0) {
        std::cerr << "Error: num_threads must be a positive integer.\n";
        return 1;
    }

    // create requested number of threads from the client 
    // -- TO-DO: Add scheduling to handle higher number of threads then max 16.
    pthread_t* threads = new pthread_t[num_threads];

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], nullptr, read_shared_memory, nullptr);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }

    delete[] threads;

    // int client_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    // char* data = (char*)mmap(0, SHM_DATA_SIZE, PROT_READ, MAP_SHARED, client_fd, 0);

    // std::cout << "Client received: " << data << std::endl;

    // munmap(data, SHM_DATA_SIZE);
    // close(client_fd);

    return 0;
}