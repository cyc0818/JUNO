INSERT INTO global_tag (
       `name`
) VALUES (
       "GT_J18_v1"
);

INSERT INTO tag (
       `name`,
       `type`
) VALUES (
       "CD.LS.MatProp.2018.V1",
       "CD.LS.MatProp"
);

INSERT INTO iov (`since`, `payload_hash`) VALUES (1234, "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d");
INSERT INTO iov (`since`, `payload_hash`) VALUES (5678, "7c211433f02071597741e6ff5a8ea34789abbf43");

INSERT INTO payload (
       `hash`,
       `type`,
       `data`,
       `streamer`
) VALUES (
       "7c211433f02071597741e6ff5a8ea34789abbf43",
       "CD.LS.MatProp",
       "",
       "DB"
);

INSERT INTO payload (
       `hash`,
       `type`,
       `data`,
       `streamer`
) VALUES (
       "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d",
       "CD.LS.MatProp",
       "",
       "DB"
);

INSERT INTO global_tag_map (
       `global_tag_name`,
       `tag_name`
) VALUES (
       "GT_J18_v1",
       "CD.LS.MatProp.2018.V1"
);

INSERT INTO tag_iov_map (`tag_name`, `iov_id`) VALUES ("CD.LS.MatProp.2018.V1", 1);
INSERT INTO tag_iov_map (`tag_name`, `iov_id`) VALUES ("CD.LS.MatProp.2018.V1", 2);
