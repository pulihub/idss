#!/usr/bin/env bash
kubectl create secret generic idsscerts --from-file=../certs/cacert.pem --from-file=../certs/client.pem --from-file=../certs/server.pem
