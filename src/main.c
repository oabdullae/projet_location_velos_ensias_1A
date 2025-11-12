#include <stdio.h>
#include "interface.h"
#include "velo.h"
#include "client.h"
#include "file_io.h"


int main() {
    printf("BONJOUR\n");
    print_client();
    print_velo();
    print_interface();
    print_fileio();
    return 0;
}
