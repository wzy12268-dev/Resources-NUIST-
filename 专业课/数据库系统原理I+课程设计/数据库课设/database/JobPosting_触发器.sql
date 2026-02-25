CREATE TRIGGER trg_CheckDateValidity
ON JobPosting
AFTER INSERT, UPDATE
AS
BEGIN
    IF EXISTS (
        SELECT 1 FROM inserted
        WHERE 截止时间 < 发布时间
    )
    BEGIN
        RAISERROR('错误：截止时间不能早于发布时间。', 16, 1);
        ROLLBACK TRANSACTION;
    END
END;
