CREATE TABLE College (
    学院编号 CHAR(4) NOT NULL PRIMARY KEY,
    学院名称 NVARCHAR(50) NOT NULL UNIQUE
);
CREATE TABLE Teacher (
    工号 VARCHAR(5) NOT NULL PRIMARY KEY,
    姓名 NVARCHAR(10) NOT NULL,
    性别 NVARCHAR(2) CHECK (性别 IN ('男', '女')),
    出生日期 DATE CHECK (出生日期 <= GETDATE()),
    联系电话 VARCHAR(11) UNIQUE,
    学院编号 CHAR(4),
    FOREIGN KEY (学院编号) REFERENCES College(学院编号)
);
CREATE TABLE Major (
    专业编号 CHAR(4) NOT NULL PRIMARY KEY,
    专业名称 NVARCHAR(20) NOT NULL UNIQUE,
    学院编号 CHAR(4),
    FOREIGN KEY (学院编号) REFERENCES College(学院编号)
);
CREATE TABLE Industry (
    行业编号 CHAR(4) NOT NULL PRIMARY KEY,
    行业名称 NVARCHAR(50) NOT NULL UNIQUE
);
CREATE TABLE Employer (
    单位编号 CHAR(4) NOT NULL PRIMARY KEY,
    单位名称 NVARCHAR(50) NOT NULL UNIQUE,
    单位地址 NVARCHAR(100),
    联系电话 VARCHAR(11),
    行业编号 CHAR(4),
    FOREIGN KEY (行业编号) REFERENCES Industry(行业编号)
);
CREATE TABLE Graduate (
    学号 VARCHAR(6) NOT NULL PRIMARY KEY,
    姓名 NVARCHAR(10) NOT NULL,
    性别 NVARCHAR(2) CHECK (性别 IN ('男', '女')),
    出生日期 DATE CHECK (出生日期 <= GETDATE()),
    联系电话 VARCHAR(11) UNIQUE,
    专业编号 CHAR(4),
    工号 VARCHAR(5),
    单位编号 CHAR(4),
    FOREIGN KEY (专业编号) REFERENCES Major(专业编号),
    FOREIGN KEY (工号) REFERENCES Teacher(工号),
    FOREIGN KEY (单位编号) REFERENCES Employer(单位编号)
);
CREATE TABLE JobPosting (
    单位编号 CHAR(4) NOT NULL,
    专业编号 CHAR(4) NOT NULL,
    发布时间 DATE NOT NULL,
    截止时间 DATE NOT NULL,
    招聘人数 INT CHECK (招聘人数 >= 0),
    PRIMARY KEY (单位编号, 专业编号, 发布时间, 截止时间),
    FOREIGN KEY (单位编号) REFERENCES Employer(单位编号),
    FOREIGN KEY (专业编号) REFERENCES Major(专业编号)
);

