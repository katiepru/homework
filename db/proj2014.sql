CREATE TABLE IF NOT EXISTS users
(
    username    varchar(255)    PRIMARY KEY,
    email       varchar(255)    NOT NULL,
    password    varchar(255)    NOT NULL
    isAdmin     integer         NOT NULL,
    isMod       integer         NOT NULL
);

CREATE TABLE IF NOT EXISTS sales_reps
(
    username    varchar(255)    REFERENCES users,
    represents  varchar(255)    NOT NULL,
    PRIMARY KEY(username)
);

CREATE TABLE IF NOT EXISTS customers
(
    username    varchar(255)    REFERENCES users,
    PRIMARY KEY(username)
);

CREATE TABLE IF NOT EXISTS messages
(
    mid         integer         PRIMARY KEY,
    to          varchar(255)    REFERENCES users,
    from        varchar(255)    REFERENCES users,
    sent_dt     datetime        NOT NULL,
    content     varchar(1024)   NOT NULL
);

CREATE TABLE IF NOT EXISTS votes
(
    username    varchar(255),   REFERENCES users,
    pid         integer         REFERENCES posts,
    isUp        integer         NOT NULL
);

CREATE TABLE IF NOT EXISTS posts
(
    pid         integer         PRIMARY KEY
    child_of    integer         REFERENCES posts
);

CREATE TABLE IF NOT EXISTS threads
(
    pid         integer         REFERENCES posts,
    th_title    varchar(255)    NOT NULL,
    belongs_to  integer         REFERENCES topics,
    PRIMARY KEY(pid)
);

CREATE TABLE IF NOT EXISTS topics
(
    tid         integer         PRIMARY KEY,
    topic_title varchar(255)    NOT NULL,
    subtopicOf  integer         REFERENCES topics
);

CREATE TABLE IF NOT EXISTS advertisements
(
    aid         integer         PRIMARY KEY,
    pname       varchar(255)    NOT NULL,
    publishedBy varchar(255)    REFERENCES sales_reps
);

CREATE TABLE IF NOT EXISTS adtags
(
    aid         integer         REFERENCES advertisements,
    keyword     varchar(255)    NOT NULL,
    PRIMARY KEY(aid, keyword)
);

CREATE TABLE IF NOT EXISTS user_interests
(
    username    varchar(255)    REFERENCES users,
    keyword     varchar(255)    NOT NULL,
    PRIMARY KEY(username, keyword)
);

CREATE TABLE IF NOT EXISTS purchases
(
    customer    varchar(255)    REFERENCES customers,
    aid         integer         REFERENCES advertisements,
    price       decimal         NOT NULL,
    pr_dt       datetime        NOT NULL,
    PRIMARY KEY(customer, aid, pr_dt)
);
