#include <string>
#include <iostream>
#include <tchar.h>
#include "sgx_urts.h"
#include "TMCtest_u.h"

#define ENCLAVE_FILE _T("TMCtest.signed.dll")
#define TMC_SIZE 580

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 2)
    cerr << "Invalid arguments" << endl;

  sgx_enclave_id_t eid;
  sgx_status_t ret = SGX_SUCCESS;
  sgx_launch_token_t token = { 0 };
  int updated = 0;

  ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
  if (ret != SGX_SUCCESS) {
    cerr << "Failed to create enclave" << endl;
    return -1;
  }

  uint32_t enc_ret;
  uint8_t seal[TMC_SIZE];
  string cmd = string(argv[1]);

  if (cmd == "create") {
    ret = create_sealed_monotonic_counter(eid, &enc_ret, seal, TMC_SIZE);
    if (ret != SGX_SUCCESS || enc_ret != SGX_SUCCESS) {
      cerr << "Failed to create monotonic counter" << endl;
      return -1;
    }

    for (int i = 0; i < TMC_SIZE; i++)
      cout << seal[i];
  }
  else if (cmd == "inc") {
    cin.read((char*)seal, TMC_SIZE);
    
    ret = increment_monotonic_counter(eid, &enc_ret, seal, TMC_SIZE);
    if (ret != SGX_SUCCESS || enc_ret != SGX_SUCCESS) {
      cerr << "Failed to increment monotonic counter" << endl;
      return -1;
    }

    for (int i = 0; i < TMC_SIZE; i++)
      cout << seal[i];
  }
  else if (cmd == "check") {
    cin.read((char*)seal, TMC_SIZE);
    uint32_t value = 0;

    ret = read_sealed_monotonic_counter(eid, &enc_ret, seal, TMC_SIZE, &value);
    if (ret != SGX_SUCCESS || enc_ret != SGX_SUCCESS) {
      cerr << "Failed to read monotonic counter" << endl;
      return -1;
    }

    cout << value;
  }
  
  if (SGX_SUCCESS != sgx_destroy_enclave(eid))
    return -1;
}