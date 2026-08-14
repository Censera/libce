#include <stdio.h>

#include <ce/ce.h>

int main(void)
{
    CeStr message;
    CeResult result = ce_str_format(&message, "Ce %s", CE_VERSION_STRING);

    if (result != CE_OK) {
        fprintf(stderr, "ce_str_format: %s\n", ce_result_name(result));
        return 1;
    }

    puts(ce_str_data(&message));
    ce_str_free(&message);
    return 0;
}
