CREATE TABLE global_tag (
       `name` varchar(100),
       PRIMARY KEY (`name`)
);

CREATE TABLE tag (
       `name` varchar(100),
       `type` varchar(100),
       PRIMARY KEY (`name`)
);

CREATE TABLE iov (
       `id` bigint(20) NOT NULL AUTO_INCREMENT,
       `since` bigint(20),
       `payload_hash` varchar(100),
       `type` varchar(100),
       PRIMARY KEY (`id`)
);

CREATE TABLE payload (
       `hash` varchar(100),
       `type` varchar(100),
       `data` blob,
       `streamer` varchar(100),
       PRIMARY KEY (`hash`)
);

CREATE TABLE global_tag_map (
       `global_tag_name` varchar(100),
       `tag_name` varchar(100),
       CONSTRAINT `gt_name_fk` FOREIGN KEY (`global_tag_name`) REFERENCES `global_tag` (`name`),
       CONSTRAINT `t_name_fk` FOREIGN KEY (`tag_name`) REFERENCES `tag` (`name`),
       INDEX `gt_name_idx` (`global_tag_name`),
       INDEX `t_name_idx` (`tag_name`)
);

CREATE TABLE tag_iov_map (
       `tag_name` varchar(100),
       `iov_id` bigint(20),
       CONSTRAINT `tim_t_name_fk` FOREIGN KEY (`tag_name`) REFERENCES `tag` (`name`),
       CONSTRAINT `tim_iov_id_fk` FOREIGN KEY (`iov_id`) REFERENCES `iov` (`id`),
       INDEX `tim_t_name_idx` (`tag_name`)
);
