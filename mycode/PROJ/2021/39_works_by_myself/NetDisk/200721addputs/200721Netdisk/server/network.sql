-- MySQL dump 10.13  Distrib 5.7.34, for Linux (x86_64)
--
-- Host: localhost    Database: net_work
-- ------------------------------------------------------
-- Server version	5.7.34-0ubuntu0.18.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `DIR_INFO`
--

DROP TABLE IF EXISTS `DIR_INFO`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `DIR_INFO` (
  `dirID` int(11) NOT NULL AUTO_INCREMENT,
  `parentDirID` int(11) NOT NULL,
  `dirName` char(20) NOT NULL,
  `cTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `userID` int(11) DEFAULT NULL,
  PRIMARY KEY (`dirID`),
  KEY `userID` (`userID`),
  KEY `parentDirID` (`parentDirID`),
  CONSTRAINT `DIR_INFO_ibfk_2` FOREIGN KEY (`userID`) REFERENCES `USER_INFO` (`userID`),
  CONSTRAINT `DIR_INFO_ibfk_3` FOREIGN KEY (`parentDirID`) REFERENCES `DIR_INFO` (`dirID`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `DIR_INFO`
--

LOCK TABLES `DIR_INFO` WRITE;
/*!40000 ALTER TABLE `DIR_INFO` DISABLE KEYS */;
INSERT INTO `DIR_INFO` VALUES (0,0,'home','2021-07-17 12:45:41',0),(7,0,'xiaobai','2021-07-17 12:52:25',25),(8,0,'123456','2021-07-17 15:48:17',33),(9,0,'xiaohei','2021-07-18 06:25:17',34),(10,0,'xiaohong','2021-07-18 07:32:45',35);
/*!40000 ALTER TABLE `DIR_INFO` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `FILE_INFO`
--

DROP TABLE IF EXISTS `FILE_INFO`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `FILE_INFO` (
  `fileID` int(11) NOT NULL AUTO_INCREMENT,
  `fileSize` mediumtext,
  `MD5` char(64) NOT NULL,
  `referCnt` int(11) DEFAULT NULL,
  PRIMARY KEY (`fileID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `FILE_INFO`
--

LOCK TABLES `FILE_INFO` WRITE;
/*!40000 ALTER TABLE `FILE_INFO` DISABLE KEYS */;
INSERT INTO `FILE_INFO` VALUES (1,'1024','1234',2);
/*!40000 ALTER TABLE `FILE_INFO` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `LOG_OPERATION`
--

DROP TABLE IF EXISTS `LOG_OPERATION`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `LOG_OPERATION` (
  `userID` int(11) DEFAULT NULL,
  `operation` char(255) DEFAULT NULL,
  `opTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `LOG_OPERATION`
--

LOCK TABLES `LOG_OPERATION` WRITE;
/*!40000 ALTER TABLE `LOG_OPERATION` DISABLE KEYS */;
INSERT INTO `LOG_OPERATION` VALUES (0,'logout','2021-07-18 12:44:59'),(0,'logout','2021-07-18 13:12:56');
/*!40000 ALTER TABLE `LOG_OPERATION` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `USER_FILE`
--

DROP TABLE IF EXISTS `USER_FILE`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `USER_FILE` (
  `dirID` int(11) DEFAULT NULL,
  `userID` int(11) DEFAULT NULL,
  `fileID` int(11) DEFAULT NULL,
  `fileName` char(64) NOT NULL,
  `cTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  KEY `dirID` (`dirID`),
  KEY `userID` (`userID`),
  KEY `fileID` (`fileID`),
  CONSTRAINT `USER_FILE_ibfk_1` FOREIGN KEY (`dirID`) REFERENCES `DIR_INFO` (`dirID`),
  CONSTRAINT `USER_FILE_ibfk_2` FOREIGN KEY (`userID`) REFERENCES `USER_INFO` (`userID`),
  CONSTRAINT `USER_FILE_ibfk_3` FOREIGN KEY (`fileID`) REFERENCES `FILE_INFO` (`fileID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `USER_FILE`
--

LOCK TABLES `USER_FILE` WRITE;
/*!40000 ALTER TABLE `USER_FILE` DISABLE KEYS */;
/*!40000 ALTER TABLE `USER_FILE` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `USER_INFO`
--

DROP TABLE IF EXISTS `USER_INFO`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `USER_INFO` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `userName` char(20) NOT NULL,
  `salt` char(8) NOT NULL,
  `cryptpasswd` char(128) NOT NULL,
  `token` char(255) NOT NULL,
  `vipLevel` tinyint(4) NOT NULL,
  `pwdDirID` int(11) DEFAULT NULL,
  PRIMARY KEY (`userID`),
  KEY `pwdDirID` (`pwdDirID`),
  CONSTRAINT `USER_INFO_ibfk_1` FOREIGN KEY (`pwdDirID`) REFERENCES `DIR_INFO` (`dirID`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `USER_INFO`
--

LOCK TABLES `USER_INFO` WRITE;
/*!40000 ALTER TABLE `USER_INFO` DISABLE KEYS */;
INSERT INTO `USER_INFO` VALUES (0,'0','0','0','0',0,0),(25,'xiaobai','Kf','S2DH6Wl0XXCu2','0',0,7),(33,'123456','r4','U3qGJIjdXLb12','0',0,8),(34,'xiaohei','a9','q3UJk14FL7pz.','0',0,9),(35,'xiaohong','ys','ANWItn1j7UIbw','0',0,10);
/*!40000 ALTER TABLE `USER_INFO` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-07-20  9:40:02
