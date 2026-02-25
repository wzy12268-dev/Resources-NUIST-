
SELECT * FROM Major
CREATE TABLE UserAccount (
    用户编号 VARCHAR(6) PRIMARY KEY,       -- 学号或工号
    密码 VARCHAR(32) NOT NULL,            -- 存储加密后的密码（MD5）
    用户类型 CHAR(1) NOT NULL CHECK (用户类型 IN ('S', 'T')) -- S=学生, T=教师
);
