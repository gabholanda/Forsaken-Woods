#include <iostream>
#include "GameManager.h"

std::size_t totalAllocated = 0;
std::size_t totalDeallocated = 0;

void* operator new(std::size_t sz) {
	totalAllocated += sz;
	return malloc(sz);
}

void operator delete(void* ptr, std::size_t sz) {
	std::cout << "Deallocating: " << sz << " bytes\n";
	totalDeallocated += sz;
	free(ptr);
}

void reportMemoryUsage() {
	std::size_t unfreedMemory = totalAllocated - totalDeallocated;
	std::cout << "Total Allocated: " << totalAllocated << " bytes\n";
	std::cout << "Total Deallocated: " << totalDeallocated << " bytes\n";
	std::cout << "Memory Leaked: " << unfreedMemory << " bytes\n";
}

int main(int argc, char* args[]) { /// Standard C-style entry point, you need to use it
	std::atexit(reportMemoryUsage);
	GameManager *ptr = new GameManager();
	bool status  = ptr->OnCreate();
	if (status == true) {
		ptr->Run();
	} else if (status == false) {
		std::cerr << "Fatal error occured. Cannot start this program" << std::endl;
	}

	delete ptr;
	return 0;

}
