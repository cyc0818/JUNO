
--
-- Table structure for Material.LS.XXX
-- Material.LS -> Material_LS
--
-- Using following command to create this table:
--   $  mysql -u username -p --host junodb1.ihep.ac.cn -D offline_db < create.sql


DROP TABLE IF EXISTS `Material_LS`;

CREATE TABLE `Material_LS` (
  `id` int NOT NULL AUTO_INCREMENT COMMENT 'primary key',
  `create_by` varchar(64) DEFAULT NULL COMMENT 'creator',
  `create_date` datetime DEFAULT NULL COMMENT 'create data',
  `remarks` varchar(255) DEFAULT NULL COMMENT 'comments',
  `del_flag` varchar(64) DEFAULT NULL COMMENT 'delete or not. 0: not delete, 1：delete',
  `version` varchar(64) DEFAULT NULL COMMENT 'version',
  `RINDEX` longblob COMMENT 'Refractive Index',
  `ABSLENGTH` longblob COMMENT 'Absorption Length',
  `ABSLENGTHwithUnits` longblob COMMENT '',
  `FASTCOMPONENT` longblob COMMENT '',
  `REEMISSIONPROB` longblob COMMENT '',
  `RAYLEIGH` longblob COMMENT '',
  `SCINTILLATIONYIELD` longblob COMMENT '',
  `RESOLUTIONSCALE` longblob COMMENT '',
  `GammaFASTTIMECONSTANT` longblob COMMENT '',
  `GammaSLOWTIMECONSTANT` longblob COMMENT '',
  `GammaYIELDRATIO` longblob COMMENT '',
  `AlphaFASTTIMECONSTANT` longblob COMMENT '',
  `AlphaSLOWTIMECONSTANT` longblob COMMENT '',
  `AlphaYIELDRATIO` longblob COMMENT '',
  `NeutronFASTTIMECONSTANT` longblob COMMENT '',
  `NeutronSLOWTIMECONSTANT` longblob COMMENT '',
  `NeutronYIELDRATIO` longblob COMMENT '',
  `scale` longblob COMMENT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LS';
