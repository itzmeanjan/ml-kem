#!/usr/bin/python

import json
import sys
import typing

ML_KEM_512_ACVP_KAT_FILE_NAME="ml_kem_512_seckeyCheck.acvp.kat"
ML_KEM_768_ACVP_KAT_FILE_NAME="ml_kem_768_seckeyCheck.acvp.kat"
ML_KEM_1024_ACVP_KAT_FILE_NAME="ml_kem_1024_seckeyCheck.acvp.kat"

def extract_and_write_ml_kem_seckeyCheck_kats(test_group: dict[str, typing.Any], write_to_file: str):
    assert test_group["testType"] == "VAL"

    with open(write_to_file, "wt") as fd:
        for test in test_group["tests"]:
            fd.write(f'pk = {test["ek"]}\n')
            fd.write(f'sk = {test["dk"]}\n')
            fd.write(f'testPassed = {test["testPassed"]}\n')
            fd.write(f'reason = {test["reason"]}\n')

            fd.write('\n')
        
        fd.flush()


def main():
    json_as_str = ''
    for line in sys.stdin:
        json_as_str += line
    
    acvp_kats = json.loads(json_as_str)

    ml_kem_512_param_set = acvp_kats["testGroups"][6]
    ml_kem_768_param_set = acvp_kats["testGroups"][8]
    ml_kem_1024_param_set = acvp_kats["testGroups"][10]

    extract_and_write_ml_kem_seckeyCheck_kats(ml_kem_512_param_set, ML_KEM_512_ACVP_KAT_FILE_NAME)
    extract_and_write_ml_kem_seckeyCheck_kats(ml_kem_768_param_set, ML_KEM_768_ACVP_KAT_FILE_NAME)
    extract_and_write_ml_kem_seckeyCheck_kats(ml_kem_1024_param_set, ML_KEM_1024_ACVP_KAT_FILE_NAME)


if __name__=='__main__':
    main()
