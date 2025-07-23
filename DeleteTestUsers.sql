-- 테스트 유저 삭제 쿼리
SET SQL_SAFE_UPDATES = 0;

-- 이제 삭제 가능
DELETE FROM `vl_db`.`player_custom`    WHERE `ID` LIKE '%test%';
DELETE FROM `vl_db`.`player_inventory` WHERE `ID` LIKE '%test%';
DELETE FROM `vl_db`.`player_info`      WHERE `ID` LIKE '%test%';
DELETE FROM `vl_db`.`player_room`      WHERE `ID` LIKE '%test%';
DELETE FROM `vl_db`.`login_info`       WHERE `ID` LIKE '%test%';

-- 다시 안전 모드 켜기 (선택사항)
SET SQL_SAFE_UPDATES = 1;