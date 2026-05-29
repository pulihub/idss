BEGIN TRANSACTION;
CREATE TABLE tb_query (
    id_query integer primary key autoincrement,
    uqi character varying(30) NOT NULL,
    value character varying(8192) NOT NULL,
    ttl integer NOT NULL,
    arrive_time double precision NOT NULL,
    sender_key character varying(50),
    local_exec smallint NOT NULL default 0,
    completed smallint NOT NULL default 0,
    sent_back smallint NOT NULL default 0,
    failed smallint NOT NULL default 0,
CONSTRAINT tb_query_uqi_key UNIQUE (uqi));
CREATE TABLE tb_user
(
    Name            TEXT    not null,
    Surname         TEXT    not null,
    user_id         integer not null
        constraint user_id__pk
            primary key autoincrement,
    Contract_Number integer not null,
    Power           REAL    not null
);
CREATE TABLE tb_consumption
(
    timestamp   REAL    not null,
    measurement REAL    not null,
    user_id     integer not null
        constraint user_id___fk
            references user
            on delete cascade,
    constraint Consumption_pk
        primary key (timestamp, user_id)
);
DELETE FROM sqlite_sequence;
INSERT INTO sqlite_sequence VALUES('user',2);
COMMIT;
