#include "boinc_api.h"
#include "filesys.h"

using namespace std;
FILE* fp;

int main() {
    int retval;
    char buf[256];
    fp = fopen("out.txt", "a");

    BOINC_OPTIONS options;
    boinc_options_defaults(options);
    options.normal_thread_priority = true;

    retval = boinc_init_options(&options);
    if (retval) {
        fprintf(stderr, "%s boinc_init returned %d\n",
            boinc_msg_prefix(buf, sizeof(buf)), retval
        );
        exit(retval);
    }

    fprintf(fp, "test\n");
    fflush(fp);
    boinc_finish(0);
    return 0;
}
