SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';

SET search_path = public, pg_catalog;

-- channels index
CREATE SEQUENCE channel_index
    START WITH 1
    INCREMENT BY 1
    MINVALUE 0
    NO MAXVALUE
    CACHE 1;

ALTER TABLE channel_index OWNER TO postgres;

-- channels table
CREATE TABLE channels (
    "id" integer DEFAULT nextval('channel_index'::regclass) NOT NULL,
    "name" text DEFAULT 'Channel '::text,
    "address" text DEFAULT '...'::text,     --IP address or DNS-name
    "protocol" text DEFAULT 'RTSP' NOT NULL,
    "port" integer DEFAULT 80 NOT NULL,
    "login" text DEFAULT '',
    "password" text DEFAULT '',
    "lastStartTime" timestamp without time zone DEFAULT '01.01.2017 0:00'::timestamp without time zone NOT NULL
);

ALTER SEQUENCE channel_index OWNED BY channels."id";
ALTER TABLE channels OWNER TO postgres;

ALTER TABLE ONLY channels
    ADD CONSTRAINT "channelID" PRIMARY KEY ("id");
