#include <iostream>

#define foo(x), "x"
int main() {
    std::cout << foo(1) << std::endl;
    // DIR *dir;
    // struct dirent *entry;
    
    // dir = opendir("/");
    // if (dir == NULL) {
    //     perror("opendir");
    //     return 1;
    // }
    
    // while ((entry = readdir(dir)) != NULL) {
    //     printf("%s\n", entry->d_name);
    // }
    
    // closedir(dir);
    
    return 0;
}
