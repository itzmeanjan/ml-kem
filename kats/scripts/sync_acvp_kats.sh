#!/bin/bash

ACVP_SERVER_REPO_PATH="ACVP-Server"
if [ -d "$ACVP_SERVER_REPO_PATH" ]; then
    echo "> $ACVP_SERVER_REPO_PATH repository exists, let's just fetch latest."

    pushd $ACVP_SERVER_REPO_PATH
    git checkout master
    git fetch
    git pull origin master
    popd
else
    echo "> $ACVP_SERVER_REPO_PATH repository doesn't exist, let's clone it."
    git clone https://github.com/usnistgov/ACVP-Server.git
fi

cat ./ACVP-Server/gen-val/json-files/ML-KEM-keyGen-FIPS203/internalProjection.json | python parse_ml_kem_keygen_acvp_kat.py
cat ./ACVP-Server/gen-val/json-files/ML-KEM-encapDecap-FIPS203/internalProjection.json | python parse_ml_kem_encaps_acvp_kat.py
cat ./ACVP-Server/gen-val/json-files/ML-KEM-encapDecap-FIPS203/internalProjection.json | python parse_ml_kem_seckeyCheck_acvp_kat.py

mv *.acvp.kat ..

echo "> Generated all NIST ACVP KATs."
