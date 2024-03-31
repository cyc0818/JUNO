CREATE TABLE `SpmsRowValid` (
  `id` varchar(20) NOT NULL,
  `TimeStart` datetime NOT NULL,
  `TimeStop` datetime NOT NULL,
  `update_time` datetime NOT NULL,
  `rollrec` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
COMMIT;
