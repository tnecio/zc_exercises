#include <iomanip>
#include <iostream>

#include <libvirt/libvirt.h>

#define table_print3(a1, a2, a3) \
std::cout << " " << std::left << std::setw(5) << (a1) << std::setw(15) << (a2) << std::setw(10) << (a3) << std::endl


void fail(const char *reason) {
    // Easy clean-up by just terminating immediately
    std::cerr << reason << std::endl;
    exit(1);
}

const char *domain_state_to_str(int state) {
    switch (state) {
        case VIR_DOMAIN_NOSTATE:
            return "no state";
        case VIR_DOMAIN_RUNNING:
            return "running";
        case VIR_DOMAIN_BLOCKED:
            return "idle";
        case VIR_DOMAIN_PAUSED:
            return "paused";
        case VIR_DOMAIN_SHUTDOWN:
            return "in shutdown";
        case VIR_DOMAIN_SHUTOFF:
            return "shut off";
        case VIR_DOMAIN_CRASHED:
            return "crashed";
        case VIR_DOMAIN_PMSUSPENDED:
            return "pmsuspended";
        default:
            return "unknown";
    }
}

int main() {
    // Connect to default URI (most likely value of env var $LIBVIRT_DEFAULT_URI)
    virConnectPtr conn = virConnectOpen(nullptr);
    if (conn == nullptr) fail("Cannot connect to hypervisor");

    virDomainPtr *domains;
    int domains_count = virConnectListAllDomains(conn, &domains, 0x0);
    if (domains_count < 0) fail("Failure when attempting to list all domains");

    table_print3("Id", "Name", "State");
    table_print3("-----", "---------------", "----------");

    for (size_t i = 0; i < domains_count; i++) {
        virDomainPtr domain = domains[i];
        const char *name = virDomainGetName(domain);
        if (name == nullptr) fail("Failure when obtaining domain name");

        unsigned int id = virDomainGetID(domain);
        if (id == (unsigned int) -1) fail("Failure when obtaining domain ID");

        int state;
        if (virDomainGetState(domain, &state, nullptr, 0x0)) {
            fail("Failure when attempting to obtain state of a domain");
        }

        table_print3(id, name, domain_state_to_str(state));

        virDomainFree(domain);
    }
    free(domains);

    if (virConnectClose(conn) < 0) {
        fail("Failure when attempting to close connection to hypervisor");
    }

    return 0;
}
