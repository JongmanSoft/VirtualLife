CREATE DATABASE  IF NOT EXISTS `vl_db` /*!40100 DEFAULT CHARACTER SET utf8mb3 */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `vl_db`;
-- MySQL dump 10.13  Distrib 8.0.41, for Win64 (x86_64)
--
-- Host: localhost    Database: vl_db
-- ------------------------------------------------------
-- Server version	8.0.41

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `kid_info`
--

DROP TABLE IF EXISTS `kid_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `kid_info` (
  `ID` int unsigned NOT NULL,
  `PREG_ID` int unsigned NOT NULL,
  `SPOUSE_ID` int unsigned NOT NULL,
  `PERSONALITY` tinyint unsigned NOT NULL,
  `HELLO_MSG` text,
  `SKIN` float DEFAULT NULL,
  `SHIRT` smallint DEFAULT NULL,
  `PANTS` smallint DEFAULT NULL,
  `SHOES` smallint DEFAULT NULL,
  `R_EYE_COLOR_HUE` float DEFAULT NULL,
  `R_EYE_COLOR_SAT` float DEFAULT NULL,
  `L_EYE_COLOR_HUE` float DEFAULT NULL,
  `L_EYE_COLOR_SAT` float DEFAULT NULL,
  `EYE_SCALE` float DEFAULT NULL,
  `PUPIL_SCALE` float DEFAULT NULL,
  `HAIR` float DEFAULT NULL,
  `HAIR_COLOR_R` float DEFAULT NULL,
  `HAIR_COLOR_G` float DEFAULT NULL,
  `HAIR_COLOR_B` float DEFAULT NULL,
  `EYE_WIDTH` float DEFAULT NULL,
  `EYE_THICK` float DEFAULT NULL,
  `EYE_SLOPE` float DEFAULT NULL,
  `NOSE_WIDTH` float DEFAULT NULL,
  `NOSE_HEIGHT` float DEFAULT NULL,
  `MOUTH_WIDTH` float DEFAULT NULL,
  `MOUTH_THICK` float DEFAULT NULL,
  `MOUTH_SLOPE` float DEFAULT NULL,
  `CHIN` float DEFAULT NULL,
  `JAW` float DEFAULT NULL,
  `HEAVY` float DEFAULT NULL,
  `FACE_WIDTH` float DEFAULT NULL,
  `EYEBROWS` smallint DEFAULT NULL,
  `GLASSES` smallint DEFAULT NULL,
  `x` float DEFAULT NULL,
  `y` float DEFAULT NULL,
  `z` float DEFAULT NULL,
  `yaw` float DEFAULT NULL,
  `name` varchar(45) DEFAULT NULL,
  `is_Kid` tinyint DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kid_info`
--

LOCK TABLES `kid_info` WRITE;
/*!40000 ALTER TABLE `kid_info` DISABLE KEYS */;
INSERT INTO `kid_info` VALUES (0,0,0,0,'방구뿡~',0.773433,0,0,0,0.0515356,0.201683,0.307177,0.102979,0.662173,1,0,0,0,0,0.294894,0.710698,0.355634,0.275593,0.854234,0.767488,0.234825,0.435881,0.355544,0.249911,0.273156,-0.25399,0,0,2418.55,7104.89,3600,104.923,'엉덩이',1),(1,0,0,2,'안녕하세요 저는 사실 점잖은 아이랍니다',0.65214,0,1,0,0.126959,0.637274,0.653679,0.0678146,0.612346,1,0,0,0,0,0.338237,0.765773,0.407352,0.339461,0.679051,0.738291,0.308367,0.273854,0.591082,0.158391,0.282124,-0.269427,1,0,4580.38,5753.16,3600,152.638,'방구',1),(2,0,0,1,'쉐이칸샹 쉐이칸샹',0.0297985,1,0,1,0.37425,0.174519,0.132527,0.0642591,0.696616,1,1,0,0,0,0.0940628,0.628558,0.137331,0.259174,0.620343,0.812507,0.315425,0.249983,0.588404,0.322273,0.304284,-0.292201,2,0,3377.91,4647.57,3600,180.621,'소나무',1),(3,0,0,1,'0xC0000005: 0xFFFFFFFFFFFFFFFF 위치를 읽는 동안 액세스 위반이 발생했습니다..',0.00518815,0,0,1,0.387188,0.171148,0.129002,0.0640583,0.7,1,2,0,0,0,0.092349,0.625507,0.129032,0.256386,0.619953,0.81341,0.317759,0.239509,0.602954,0.327128,0.30459,-0.29461,0,0,2616.13,5368.89,3600,174.787,'비주얼스튜디오',1),(4,0,0,2,'언리얼보다 유니티가 짱이야!',0.638748,1,1,0,0.244448,0.803829,0.294527,0.0744609,0.547426,1,4,0,0,0,0.305937,0.645147,0.307601,0.344073,0.766996,0.616288,0.301728,0.370225,0.340677,0.165035,0.285911,-0.165977,1,0,4535.44,6772.58,3600,143.266,'유니티',1),(5,0,0,0,'유니티보다 언리얼이 짱이야!',0.0348754,1,1,1,0.228979,0.523391,0.690458,0.108384,0.519101,1,2,0,0,0,0.385914,0.84674,0.286516,0.299777,0.808415,0.65705,0.284685,0.264898,0.16737,0.307083,0.271324,-0.263349,2,0,957.033,7725.96,3600,36.3439,'언리얼',1),(6,0,0,1,'집에가고싶다~',0.185959,1,0,0,0.230371,0.312222,0.483788,0.0848308,0.461321,1,2,0,0,0,0.112445,0.671116,0.230886,0.325082,0.639737,0.77923,0.222977,0.60264,0.437447,0.0856407,0.303988,-0.175689,2,0,3521.28,4452.87,3600,146.562,'집',1),(7,0,0,2,'엔진보단 다렉이 짱이야!',0.209735,0,1,1,0.187479,0.531247,0.346552,0.0715103,0.609504,1,1,0,0,0,0.488985,0.794844,0.364462,0.312322,0.830216,0.668345,0.303062,0.60413,0.463376,0.119138,0.303404,-0.0777156,0,0,4906.4,5892.61,3600,337.818,'다이렉트엑스',1),(8,0,0,1,'똥먹어라퉤퉤퉤',0.385905,1,0,1,0.126286,0.188208,0.434341,0.110036,0.491675,1,4,0,0,0,0.39581,0.732853,0.139641,0.3283,0.733933,0.789014,0.281092,0.451809,0.212736,0.18405,0.294425,-0.272634,0,0,4047.64,4293.56,3600,165.602,'똥쟁이',1),(9,0,0,0,'저는강 아지입 니다.',0.198285,1,1,1,0.360554,0.317559,0.150253,0.064765,0.646198,1,2,0,0,0,0.159797,0.628931,0.169325,0.270029,0.688071,0.806842,0.306548,0.324301,0.594822,0.321349,0.298853,-0.287195,2,0,1570.37,7577.96,3600,115.019,'푸딩이',1);
/*!40000 ALTER TABLE `kid_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `login_info`
--

DROP TABLE IF EXISTS `login_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `login_info` (
  `ID` varchar(20) NOT NULL,
  `PW` varchar(20) NOT NULL,
  `IS_PLAYING` tinyint NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `login_info`
--

LOCK TABLES `login_info` WRITE;
/*!40000 ALTER TABLE `login_info` DISABLE KEYS */;
INSERT INTO `login_info` VALUES ('hello','0000',0),('jongman','0000',0),('qwerty','0000',0),('root','0000',0),('seyoung','0000',0);
/*!40000 ALTER TABLE `login_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_custom`
--

DROP TABLE IF EXISTS `player_custom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_custom` (
  `ID` varchar(20) NOT NULL,
  `SKIN` float DEFAULT NULL,
  `SHIRT` smallint DEFAULT NULL,
  `PANTS` smallint DEFAULT NULL,
  `SHOES` smallint DEFAULT NULL,
  `R_EYE_COLOR_HUE` float DEFAULT NULL,
  `R_EYE_COLOR_SAT` float DEFAULT NULL,
  `L_EYE_COLOR_HUE` float DEFAULT NULL,
  `L_EYE_COLOR_SAT` float DEFAULT NULL,
  `EYE_SCALE` float DEFAULT NULL,
  `PUPIL_SCALE` float DEFAULT NULL,
  `HAIR` smallint DEFAULT NULL,
  `HAIR_COLOR_R` float DEFAULT NULL,
  `HAIR_COLOR_G` float DEFAULT NULL,
  `HAIR_COLOR_B` float DEFAULT NULL,
  `EYE_WIDTH` float DEFAULT NULL,
  `EYE_THICK` float DEFAULT NULL,
  `EYE_SLOPE` float DEFAULT NULL,
  `NOSE_WIDTH` float DEFAULT NULL,
  `NOSE_HEIGHT` float DEFAULT NULL,
  `MOUTH_WIDTH` float DEFAULT NULL,
  `MOUTH_THICK` float DEFAULT NULL,
  `MOUTH_SLOPE` float DEFAULT NULL,
  `CHIN` float DEFAULT NULL,
  `JAW` float DEFAULT NULL,
  `HEAVY` float DEFAULT NULL,
  `FACE_WIDTH` float DEFAULT NULL,
  `EYEBROWS` smallint DEFAULT '0',
  `GLASSES` smallint DEFAULT '0',
  PRIMARY KEY (`ID`),
  CONSTRAINT `LINFO_PCUST` FOREIGN KEY (`ID`) REFERENCES `login_info` (`ID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_custom`
--

LOCK TABLES `player_custom` WRITE;
/*!40000 ALTER TABLE `player_custom` DISABLE KEYS */;
INSERT INTO `player_custom` VALUES ('hello',0.00518815,0,0,1,0.387188,0.171148,0.129002,0.0640583,0.7,1,3,0,0,0,0.092349,0.625507,0.129032,0.256386,0.619953,0.81341,0.317759,0.239509,0.602954,0.327128,0.30459,-0.29461,0,0),('jongman',1,0,0,1,0.686917,0,0.686917,0,0.358232,0.9,3,0.0982776,0.0269907,0,0.546,-0.0793376,0.132999,-0.207644,0.342967,0.144661,0.466683,0,0,0,0,0,2,0),('qwerty',0.0103763,0,1,1,0.296884,0.691366,0.580798,0.890561,0.481011,0.892953,0,0,0,0,0.55916,0.147435,0.621052,0.632618,0.356945,0.238411,0.936003,0.801202,0.880276,0.473952,0.067629,0.0188452,2,1),('root',0.847407,0,1,1,0.000549333,0.831141,0.69216,0.115024,0.419514,1,2,0,0,0,0.510178,0.85699,0.439406,0.35077,0.906034,0.607227,0.197638,0.664174,0.0917692,0.0844036,0.269036,-0.0757317,2,0),('seyoung',0.5,0,0,0,1,1,1,1,0.35,0.9,0,1,0.2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
/*!40000 ALTER TABLE `player_custom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_info`
--

DROP TABLE IF EXISTS `player_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_info` (
  `ID` varchar(20) NOT NULL,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `POS_X` double DEFAULT NULL,
  `POS_Y` double DEFAULT NULL,
  `POS_Z` double DEFAULT NULL,
  `YAW` double DEFAULT NULL,
  `LAST_LOGIN` datetime DEFAULT NULL,
  `MONEY` int DEFAULT '0',
  PRIMARY KEY (`ID`),
  CONSTRAINT `ID` FOREIGN KEY (`ID`) REFERENCES `login_info` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_info`
--

LOCK TABLES `player_info` WRITE;
/*!40000 ALTER TABLE `player_info` DISABLE KEYS */;
INSERT INTO `player_info` VALUES ('hello','김이름',530.239501953125,1719.4873046875,3159.041748046875,65.9749984741211,'2025-07-23 21:29:05',1500),('jongman','이종만',1097.436767578125,1761.1468505859375,3200,0,'2025-07-22 22:00:49',1500),('qwerty','김테스트',1350.1163330078125,1550.2076416015625,3123.2802734375,-29.069793701171875,'2025-07-22 01:20:12',380),('root','가나다',1513.2138671875,4367.00146484375,3263.4453125,54.75,'2025-07-23 21:29:05',478),('seyoung','김겜공',1.7822961371163534e-21,9.327042578545982e-41,-459561500672,-1.2142180311564541e-29,'2025-07-22 22:30:52',1380);
/*!40000 ALTER TABLE `player_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_inventory`
--

DROP TABLE IF EXISTS `player_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_inventory` (
  `ID` varchar(20) NOT NULL,
  `ITEM_CODE` int NOT NULL,
  `ITEM_COUNT` int DEFAULT '0',
  PRIMARY KEY (`ID`,`ITEM_CODE`),
  CONSTRAINT `LINFO_PINVT_ID` FOREIGN KEY (`ID`) REFERENCES `login_info` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_inventory`
--

LOCK TABLES `player_inventory` WRITE;
/*!40000 ALTER TABLE `player_inventory` DISABLE KEYS */;
INSERT INTO `player_inventory` VALUES ('hello',8,1),('hello',9,1),('hello',10,1),('hello',11,1),('jongman',8,1),('jongman',9,1),('jongman',10,1),('jongman',11,1),('qwerty',8,1),('qwerty',9,1),('qwerty',10,1),('qwerty',11,1),('root',0,137),('root',1,54),('root',2,3),('root',3,1),('root',8,1),('root',9,1),('root',10,1),('root',11,1),('seyoung',8,1),('seyoung',9,1),('seyoung',10,1),('seyoung',11,1);
/*!40000 ALTER TABLE `player_inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_quest`
--

DROP TABLE IF EXISTS `player_quest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_quest` (
  `ID` varchar(20) NOT NULL,
  `QUEST_ID` int NOT NULL,
  PRIMARY KEY (`ID`,`QUEST_ID`),
  CONSTRAINT `LOGIN_PQUEST_ID` FOREIGN KEY (`ID`) REFERENCES `login_info` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_quest`
--

LOCK TABLES `player_quest` WRITE;
/*!40000 ALTER TABLE `player_quest` DISABLE KEYS */;
INSERT INTO `player_quest` VALUES ('root',4),('root',5);
/*!40000 ALTER TABLE `player_quest` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `player_room`
--

DROP TABLE IF EXISTS `player_room`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_room` (
  `ID` varchar(20) NOT NULL,
  `ITEM_ID` smallint unsigned NOT NULL,
  `POS_X` float NOT NULL,
  `POS_Y` float NOT NULL,
  `POS_Z` float NOT NULL,
  `SCALE` float NOT NULL,
  `YAW` float NOT NULL,
  PRIMARY KEY (`ID`,`ITEM_ID`,`POS_X`,`POS_Y`,`POS_Z`),
  CONSTRAINT `USER_ID` FOREIGN KEY (`ID`) REFERENCES `login_info` (`ID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `player_room`
--

LOCK TABLES `player_room` WRITE;
/*!40000 ALTER TABLE `player_room` DISABLE KEYS */;
INSERT INTO `player_room` VALUES ('hello',0,-2550,-8610,3304,1,0),('hello',0,-2350,-8550,3304,1,0),('hello',0,-2250,-8550,3304,1,0),('hello',1,-2590,-8520,3304.15,1,0),('hello',1,-2490,-8220,3304.15,1,0),('hello',6,-2830,-8510,3304,1,0),('jongman',65535,0,0,0,0,0),('qwerty',65535,0,0,0,0,0),('root',0,-2550,-8610,3304,1,0),('root',0,-2350,-8550,3304,1,0),('root',0,-2250,-8550,3304,1,0),('root',1,-2590,-8520,3304.15,1,0),('root',1,-2490,-8220,3304.15,1,0),('root',1,-2400,-8340,3304.15,1,0),('root',3,-2920,-8200,3304.55,1,0),('root',3,-1950,-9160,3304.55,1,0),('root',6,-2830,-8510,3304,1,0),('root',14,-2680,-8380,3304,1,0),('root',65535,0,0,0,0,0),('seyoung',0,-2340,-8800,3303.96,1,0),('seyoung',0,-2290,-8540,3303.96,1.2,30),('seyoung',0,-1920,-8250,3303.96,0.2,0),('seyoung',0,-1830,-8250,3303.96,0.6,0),('seyoung',0,-1800,-10020,3303.96,1,0),('seyoung',0,-1700,-8250,3303.96,1,0),('seyoung',0,-1540,-8250,3303.96,1.4,0),('seyoung',0,-1330,-8260,3303.96,2.4,0),('seyoung',2,-1420,-8680,3304,1.6,0),('seyoung',2,-1320,-8680,3304,1.7,0),('seyoung',3,-3330,-9450,3304.55,1.7,-90),('seyoung',3,-1480,-8470,3304.55,1.2,90),('seyoung',4,-2130,-10020,3304,1,0),('seyoung',5,-3120,-8580,3304,1.7,0),('seyoung',6,-2280,-8510,3304,1.2,-90),('seyoung',11,-3300,-8410,3303.92,1.4,80),('seyoung',11,-3030,-9970,3303.92,1.6,-90),('seyoung',11,-3020,-10100,3303.92,1.7,-90),('seyoung',12,-3080,-9160,3309.64,1.8,60),('seyoung',12,-2070,-8100,3304,1.2,50),('seyoung',14,-2480,-10190,3540.05,1,0),('seyoung',15,-2470,-10190,3459.37,1,0),('seyoung',15,-1380,-9600,3491.32,1.2,0),('seyoung',15,-1360,-9000,3413.11,1.2,0),('seyoung',17,-2520,-10190,3458.89,1,0),('seyoung',18,-2940,-10190,3409.79,1,0),('seyoung',23,-1960,-9340,3466.07,1.5,0),('seyoung',23,-1390,-8600,3402.22,1.8,0),('seyoung',24,-2180,-9340,3492.13,1.3,0),('seyoung',26,-3290,-10210,3349.8,1.9,0),('seyoung',26,-3150,-10210,3371.33,1.8,0),('seyoung',26,-2350,-8580,3353.53,2,90),('seyoung',26,-2350,-8470,3326.2,2.1,90),('seyoung',27,-3480,-8410,3304.83,1.7,-90),('seyoung',27,-3470,-10040,3304.83,2.2,-90),('seyoung',27,-2210,-8080,3304.83,1.3,-90),('seyoung',28,-1990,-8690,3304,1.2,180),('seyoung',29,-2350,-10100,3303.63,1.3,90),('seyoung',30,-2770,-8410,3304.77,2.8,0),('seyoung',30,-1440,-10100,3304.77,1,-90),('seyoung',31,-3410,-8830,3305.56,1.5,0),('seyoung',31,-2810,-9530,3305.56,2.1,90),('seyoung',32,-3370,-7980,3304.02,1.7,0),('seyoung',32,-3180,-7980,3304.02,1.7,0),('seyoung',32,-3010,-7970,3304.02,1.7,0),('seyoung',32,-2230,-9290,3304.02,2.2,0),('seyoung',33,-3250,-9580,3304.02,1.5,-90),('seyoung',33,-2540,-9510,3304.02,1.4,110),('seyoung',33,-2260,-9620,3304.02,1.5,-80),('seyoung',33,-1460,-9620,3304.02,1.4,90),('seyoung',34,-2910,-9850,3304.08,1.6,0),('seyoung',34,-1870,-9820,3304.08,1.7,0),('seyoung',35,-2190,-8970,3304.14,1.4,40),('seyoung',43,-1300,-10000,3476.08,3,90),('seyoung',43,-1300,-8930,3522.94,3,100),('seyoung',44,-2740,-10210,3495.31,3,0),('seyoung',44,-2290,-9630,3508.8,2.6,90),('seyoung',44,-1730,-7950,3485.17,2.7,0),('seyoung',44,-1290,-9230,3510.51,3,90),('seyoung',45,-1990,-10210,3496.95,2.1,0),('seyoung',45,-1520,-9340,3571.25,2,0),('seyoung',46,-2290,-9780,3502.28,2.3,90),('seyoung',46,-1300,-9070,3463.5,3,90),('seyoung',47,-3520,-9200,3426.72,1.6,-90),('seyoung',47,-2490,-10210,3453.5,1.3,0),('seyoung',48,-3090,-8280,3304.35,1.2,50),('seyoung',49,-2920,-10210,3309.2,1.3,0),('seyoung',50,-2990,-9530,3304,1.4,0),('seyoung',50,-2890,-9540,3304,1.3,0),('seyoung',50,-2770,-9530,3304,1.5,0),('seyoung',50,-1800,-9860,3304,1.3,0),('seyoung',50,-1360,-9000,3304,1.4,90),('seyoung',51,-1430,-9340,3517.39,1.8,0),('seyoung',51,-1300,-9830,3508.06,2,100),('seyoung',51,-1300,-8290,3485.14,2.6,100),('seyoung',52,-1580,-10130,3307.2,1,0),('seyoung',53,-1720,-10200,3304,1,0),('seyoung',54,-1370,-9550,3304,1.5,90),('seyoung',54,-1370,-8080,3304,1.3,90),('seyoung',55,-2010,-8600,3528.08,1.4,0),('seyoung',56,-1440,-10170,3304.16,1,0),('seyoung',57,-3581.89,-10236.5,3304,45.9359,0),('seyoung',57,-3573.69,-7916.92,3317.98,13.0767,0),('seyoung',57,-3568.1,-8751.86,3304,16.3713,90),('seyoung',57,-3540.03,-10195.8,3304,23.8401,90),('seyoung',57,-3540,-8720,3309.6,1,0),('seyoung',57,-3500,-9000,3308.1,1,0),('seyoung',57,-2672,-7922.52,3304,27.4587,0),('seyoung',57,-2410,-8190,3304,1,0),('seyoung',57,-2335.88,-8213.57,3304,5.12125,90),('seyoung',57,-2323.64,-8612.06,3304,4.14118,90),('seyoung',57,-2265.09,-10180.1,3304,15.9913,90),('seyoung',57,-2248.87,-9364.5,3319.61,6.91054,0),('seyoung',57,-1642.17,-9369,3304,7.85128,0),('seyoung',57,-1371.53,-8621.01,3304,19.0083,180),('seyoung',57,-1334.1,-8621.34,3304,1.16036,180),('seyoung',57,-1267.28,-10204.8,3304,45.1154,90);
/*!40000 ALTER TABLE `player_room` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-07-24  0:15:17
