BEGIN TRANSACTION;
CREATE TABLE tb_system (
id_sys integer primary key autoincrement,
name character(50) NOT NULL,
release character(50) NOT NULL,
"version" character(256) NOT NULL,
machine character(30) NOT NULL,
creation_date bigint NOT NULL,
validity_time numeric NOT NULL);
DELETE FROM sqlite_sequence;
CREATE TABLE tb_firewall (
id_firewall integer primary key autoincrement,
hostname character(256) unique NOT NULL,
creation_date bigint NOT NULL,
validity_time numeric NOT NULL);
CREATE TABLE tb_host (
id_host integer primary key autoincrement,
host_name character(256) unique,
domain_name character(256) NOT NULL,
    id_hos_sys integer,
    id_hos_fir integer,
constraint "$1" FOREIGN KEY (id_hos_sys) REFERENCES tb_system(id_sys),
constraint "$2" FOREIGN KEY (id_hos_fir) REFERENCES tb_firewall(id_firewall));
CREATE TABLE tb_cpu_static (
id_cpu_s integer primary key autoincrement,
number integer NOT NULL,
    vendor character(128) NOT NULL,
    "version" character(32) NOT NULL,
    model character(64) NOT NULL,
    cpu_mhz real NOT NULL,
    cache_size integer NOT NULL,
    features character(256),
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
    id_cpu_s_hos integer NOT NULL,
constraint "$1" foreign key (id_cpu_s_hos) references tb_host(id_host));
CREATE TABLE tb_net (
    id_hos_net_inf integer NOT NULL,
    name character(64) NOT NULL,
    address character(15) NOT NULL,
    mask character(15),
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT tb_net_pkey PRIMARY KEY (id_hos_net_inf, name),
CONSTRAINT "$1" FOREIGN KEY (id_hos_net_inf) REFERENCES tb_host(id_host));
CREATE TABLE tb_device_static (
    id_hos_dev_s integer NOT NULL,
    name character(256) NOT NULL,
    mount_point character(256) NOT NULL,
    fs_type character(32) NOT NULL,
    size bigint NOT NULL,
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT tb_device_static_pkey PRIMARY KEY (id_hos_dev_s, name),
CONSTRAINT "$1" FOREIGN KEY (id_hos_dev_s) REFERENCES tb_host(id_host));
CREATE TABLE tb_device_dynamic (
    id_hos_dev_d integer NOT NULL,
    name character(256) NOT NULL,
    free_space bigint NOT NULL,
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT tb_device_dynamic_pkey PRIMARY KEY (id_hos_dev_d, name),
CONSTRAINT "$1" FOREIGN KEY (id_hos_dev_d) REFERENCES tb_host(id_host));
CREATE TABLE tb_memory_static (
    id_hos_mem_s integer NOT NULL,
    total_ram numeric(10, 2) NOT NULL,
    total_swap numeric(20, 2),
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT tb_memory_static_pkey PRIMARY KEY (id_hos_mem_s),
CONSTRAINT "$1" FOREIGN KEY (id_hos_mem_s) REFERENCES tb_host(id_host));
CREATE TABLE tb_memory_dynamic (
    id_hos_mem_d integer NOT NULL,
    free_ram numeric(10, 2) NOT NULL,
    free_swap numeric(10, 2),
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT tb_memory_dynamic_pkey PRIMARY KEY (id_hos_mem_d),
CONSTRAINT "$1" FOREIGN KEY (id_hos_mem_d) REFERENCES tb_host(id_host));
CREATE TABLE tb_cpu_dynamic (
    id_hos_cpu_d integer NOT NULL,
    number integer NOT NULL,
    "load" real NOT NULL,
    user_time real,
    nice_time real,
    system_time real,
    idle_time real,
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT tb_cpu_dynamic_pkey PRIMARY KEY (id_hos_cpu_d, number),
CONSTRAINT "$1" FOREIGN KEY (id_hos_cpu_d) REFERENCES tb_host(id_host));
CREATE TABLE tb_fir_ports (
    id_fir_por integer NOT NULL,
    ports character(13) NOT NULL,
    protocol character(16) NOT NULL,
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
    type_por character(1) NOT NULL,
CONSTRAINT tb_fir_ports_pkey PRIMARY KEY (id_fir_por, ports, protocol),
CONSTRAINT "$1" FOREIGN KEY (id_fir_por) REFERENCES tb_firewall(id_firewall) ON DELETE CASCADE);
CREATE TABLE tb_lrms (
id_lrms integer primary key autoincrement,
    "type" character(32) NOT NULL,
    "version" character(32),
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
    hostname character(256) NOT NULL,
    id_hos_lrm integer NOT NULL,
CONSTRAINT "$1" FOREIGN KEY (id_hos_lrm) REFERENCES tb_host(id_host));
CREATE TABLE tb_queue (
    id_queue integer primary key autoincrement,
name character(256) NOT NULL,
    asscpunum integer,
    status character(32),
    maxqueue integer,
    maxrun integer,
    queued integer,
    running integer,
    maxwct integer,
    maxcput integer,
    hostname character(256) NOT NULL,
    id_lrm_que integer NOT NULL,
CONSTRAINT "$1" FOREIGN KEY (id_lrm_que) REFERENCES tb_lrms(id_lrms) ON DELETE CASCADE);
CREATE TABLE tb_job (
    id_job integer primary key autoincrement,
name character(256) NOT NULL,
    job_id character(64) NOT NULL,
    job_id2 character(64),
    "owner" character(256),
    status character(32),
    reqcput integer,
    subtime bigint,
    usedcput integer,
    usedmem integer,
    usedwt integer,
    estimatert integer,
    hostname character(256) NOT NULL,
    description varchar(512),
    id_que_job integer NOT NULL,
CONSTRAINT "$1" FOREIGN KEY (id_que_job) REFERENCES tb_queue(id_queue) ON DELETE CASCADE);
CREATE TABLE tb_service (
    id_service integer primary key autoincrement,
    name character(256),
    description character varying(1024),
    keywords character(256),
    def_port integer NOT NULL,
CONSTRAINT tb_service_name_key UNIQUE (name));
CREATE TABLE rl_hos_service (
    id_rl_hos integer NOT NULL,
    id_rl_ser integer NOT NULL,
    accessurl character(256) NOT NULL,
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT rl_hos_service_pkey PRIMARY KEY (id_rl_hos, id_rl_ser, accessurl),
CONSTRAINT "$1" FOREIGN KEY (id_rl_hos) REFERENCES tb_host(id_host),
CONSTRAINT "$2" FOREIGN KEY (id_rl_ser) REFERENCES tb_service(id_service));
CREATE TABLE tb_web_service (
    id_web_service integer primary key autoincrement,
    name character(256),
    description character varying(1024),
    keywords character(256),
CONSTRAINT tb_web_service_name_key UNIQUE (name));
CREATE TABLE tb_ws_wsdl_loc (
    id_ws_wsdl integer NOT NULL,
    wsdl_locationurl character(256) NOT NULL,
CONSTRAINT tb_ws_wsdl_loc_pkey PRIMARY KEY (id_ws_wsdl, wsdl_locationurl),
CONSTRAINT "$1" FOREIGN KEY (id_ws_wsdl) REFERENCES tb_web_service(id_web_service) ON DELETE CASCADE);
CREATE TABLE rl_hos_ws (
    id_rl_hos integer NOT NULL,
    id_rl_ws integer NOT NULL,
    accessurl character(256) NOT NULL,
    creation_date bigint NOT NULL,
    validity_time numeric NOT NULL,
CONSTRAINT rl_hos_ws_pkey PRIMARY KEY (id_rl_hos, id_rl_ws, accessurl),
CONSTRAINT "$1" FOREIGN KEY (id_rl_hos) REFERENCES tb_host(id_host),
CONSTRAINT "$2" FOREIGN KEY (id_rl_ws) REFERENCES tb_web_service(id_web_service));
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
CREATE UNIQUE INDEX tb_system_name_key ON tb_system (name, release, "version", machine);
CREATE UNIQUE INDEX tb_cpu_static_id_cpu_s_hos_key ON tb_cpu_static (id_cpu_s_hos);
CREATE UNIQUE INDEX tb_lrms_type_key ON tb_lrms ("type", hostname);
CREATE UNIQUE INDEX tb_queue_name_key ON tb_queue (name, hostname);
CREATE UNIQUE INDEX tb_job_job_id_key ON tb_job(job_id, hostname);
COMMIT;
