DROP TABLE UserAccount;

CREATE TABLE UserAccount (
    用户编号 VARCHAR(6) PRIMARY KEY,
    密码 VARCHAR(32) NOT NULL,
    用户类型 CHAR(1) NOT NULL CHECK (用户类型 IN ('S', 'T', 'A')) -- A=管理员
);
INSERT INTO UserAccount (用户编号, 密码, 用户类型) VALUES ('admin', 'admin123', 'A');
INSERT INTO UserAccount (用户编号, 密码, 用户类型) VALUES ('T001','122122','T')
INSERT INTO UserAccount (用户编号, 密码, 用户类型) VALUES ('S10016','122122','S')