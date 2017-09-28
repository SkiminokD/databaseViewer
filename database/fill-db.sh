#!/bin/bash
# TODO: Move host, port, username and password to parameters
psql --host=localhost --port=5432 --username=postgres --file=data.sql "inobitec"
