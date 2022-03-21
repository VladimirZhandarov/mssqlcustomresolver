DECLARE @publicationVar varchar(60)
SET @publicationVar = 'UPE_MM2_Administration2_Publication'
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admActivity', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmActivity_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admActivity', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admActivityModule', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmActivityModule_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admActivityModule', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admActivityUsingHistory', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmActivityUsingHistory_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admActivityUsingHistory', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admAircraftAccess', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmAircraftAccess_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admAircraftAccess', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admRole', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmRole_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admRole', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admRoleAccess', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmRoleAccess_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admRoleAccess', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admSystemEventAccount', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmSystemEventAccount_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admSystemEventAccount', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserAccess', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmUserAccess_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserAccess', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserInRole', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmUserInRole_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserInRole', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserLogs', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmUserLogs_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserLogs', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserPassword', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmUserPassword_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admUserPassword', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admVacancyRoles', @property = 'article_resolver', @value = 'UPE_MM2_Administration2_AdmVacancyRoles_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'admVacancyRoles', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_helpmergearticle;
