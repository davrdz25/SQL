CREATE TABLE [Users] (
    UserEntry INT NOT NULL,
    UserCode NVARCHAR(20) NOT NULL,
    FirstName NVARCHAR(50) NOT NULL,
    MiddleName NVARCHAR(50),
    LastName NVARCHAR(50) NOT NULL,
    Phone NVARCHAR(50) NOT NULL,
    Email NVARCHAR(50) NOT NULL,
    [Password] VARBINARY(128) NOT NULL,
    CONSTRAINT PK_Users_UserEntry PRIMARY KEY (UserEntry),
    CONSTRAINT UQ_Users_UserCode UNIQUE (UserCode),
    CONSTRAINT UQ_Users_UserName UNIQUE (FirstName, LastName),
    CONSTRAINT UQ_Users_Phone UNIQUE (Phone),
    CONSTRAINT UQ_Users_Email UNIQUE (Email)
)