#!/bin/bash
if [ $# -lt  2 ]
then
    echo "Usages: create-db.sh [port] [pglogin] [pgpassword]"
    echo "Try use default parameters:"
    echo "  port = 5432"
    echo "  pglogin = postgres"
    echo "  pgpassword is empty"
    PORT=5432
    PGLOGIN=postgres
else
    PORT=$1
    PGLOGIN=$2
fi

if [ $# -eq  3 ]
then
    export PGPASSWORD=$3
fi

createdb --host=localhost --port=$PORT --username=$PGLOGIN "inobitec"
psql --host=localhost --port=$PORT --username=$PGLOGIN --file=schema.sql "inobitec"
