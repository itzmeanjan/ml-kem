#!/usr/bin/python

import json
import sys
import typing

ML_KEM_512_ACVP_KAT_FILE_NAME="ml_kem_512_encaps_key_check.acvp.kat"
ML_KEM_768_ACVP_KAT_FILE_NAME="ml_kem_768_encaps_key_check.acvp.kat"
ML_KEM_1024_ACVP_KAT_FILE_NAME="ml_kem_1024_encaps_key_check.acvp.kat"

def extract_and_write_ml_kem_encaps_key_check_kats(test_group: dict[str, typing.Any], write_to_file: str):
    assert test_group["testType"] == "VAL"
    assert test_group["function"] == "encapsulationKeyCheck"

    with open(write_to_file, "wt") as fd:
        for test in test_group["tests"]:
            fd.write(f'pk = {test["ek"]}\n')
            fd.write(f'testPassed = {test["testPassed"]}\n')
            fd.write(f'reason = {test["reason"]}\n')

            fd.write('\n')

        fd.flush()


def main():
    json_as_str = ''
    for line in sys.stdin:
        json_as_str += line

    acvp_kats = json.loads(json_as_str)

    ml_kem_512_param_set = acvp_kats["testGroups"][7]
    ml_kem_768_param_set = acvp_kats["testGroups"][9]
    ml_kem_1024_param_set = acvp_kats["testGroups"][11]

    extract_and_write_ml_kem_encaps_key_check_kats(ml_kem_512_param_set, ML_KEM_512_ACVP_KAT_FILE_NAME)
    extract_and_write_ml_kem_encaps_key_check_kats(ml_kem_768_param_set, ML_KEM_768_ACVP_KAT_FILE_NAME)
    extract_and_write_ml_kem_encaps_key_check_kats(ml_kem_1024_param_set, ML_KEM_1024_ACVP_KAT_FILE_NAME)


if __name__=='__main__':
    main()
