
--Участвующие базы
--UPE_MM2_Administration2
--UPE_MM2_Catalog
--UPE_MM2_Documents2
--UPE_MM2_Formular
--UPE_MM2_ItemsAccount
--UPE_MM2_JPS
--UPE_MM2_Organization
--UPE_MM2_Planning
--UPE_MM2_Reglament2


use master
GO

DECLARE @UserName nvarchar(100);
DECLARE @UserPassword nvarchar(100);

SET @UserName='upedb';
SET @UserPassword='upem29'
--1. Создать логин в экземпл¤ре 
EXECUTE('CREATE LOGIN '+@UserName+' WITH PASSWORD = '''+@UserPassword+''' , DEFAULT_LANGUAGE = [English]'); 

--2.Создать пользователя для базы    
DECLARE @DBName nvarchar(100);

----------
SET @DBName='UPE_MM2_Administration2';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_Catalog';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_Documents2';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_Formular';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_ItemsAccount';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_JPS';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_Organization';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_Planning';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; ');   

----------
SET @DBName='UPE_MM2_Reglament2';
EXECUTE('USE '+@DBName+' CREATE USER '+@UserName+' FOR LOGIN '+@UserName);   
EXECUTE('USE '+@DBName+' EXEC sp_addrolemember ''DB_OWNER'', '''+@UserName+'''; '); 


