#!/bin/bash
# TODO: Move host, port, username and password to parameters
createdb --host=localhost --port=5432 --username=postgres "inobitec"
psql --host=localhost --port=5432 --username=postgres --file=schema.sql "inobitec"
