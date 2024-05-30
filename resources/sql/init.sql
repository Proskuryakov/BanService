CREATE TABLE bans
(
    resource_id     BIGINT       NOT NULL,
    user_id         BIGINT       NOT NULL,
    resource_type   VARCHAR(255) NOT NULL,
    reason          TEXT,
    moderator_id    BIGINT       NOT NULL,
    created_at      BIGINT       NOT NULL,
    updated_at      BIGINT       NOT NULL,
    expiration_date BIGINT       NOT NULL,
    PRIMARY KEY (resource_id, user_id, resource_type)
);