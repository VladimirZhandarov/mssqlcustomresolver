DECLARE @publicationVar varchar(60)
SET @publicationVar = 'UPE_MM2_ItemsAccount_Publication'
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Account_Items', @property = 'article_resolver', @value = 'UPE_MM2_ItemsAccount_Account_Items_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Account_Items', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Account_SNOKPA', @property = 'article_resolver', @value = 'UPE_MM2_ItemsAccount_Account_SNOKPA_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Account_SNOKPA', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Account_SupportItems', @property = 'article_resolver', @value = 'UPE_MM2_ItemsAccount_Account_SupportItems_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Account_SupportItems', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'FAP_A50_1Account_Aircrafts', @property = 'article_resolver', @value = 'UPE_MM2_ItemsAccount_FAP_A50_1Account_Aircrafts_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'FAP_A50_1Account_Aircrafts', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'FAP_A50_2Account_Engines', @property = 'article_resolver', @value = 'UPE_MM2_ItemsAccount_FAP_A50_2Account_Engines_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'FAP_A50_2Account_Engines', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'FAP_A50_3Account_DetachItems', @property = 'article_resolver', @value = 'UPE_MM2_ItemsAccount_FAP_A50_3Account_DetachItems_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'FAP_A50_3Account_DetachItems', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_helpmergearticle;
