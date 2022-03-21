DECLARE @publicationVar varchar(60)
SET @publicationVar = 'UPE_MM2_Formular_Publication'
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'AircraftBaseReplace', @property = 'article_resolver', @value = 'UPE_MM2_Formular_AircraftBaseReplace_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'AircraftBaseReplace', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'AttentionPointsInService', @property = 'article_resolver', @value = 'UPE_MM2_Formular_AttentionPointsInService_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'AttentionPointsInService', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'BasementPlaces', @property = 'article_resolver', @value = 'UPE_MM2_Formular_BasementPlaces_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'BasementPlaces', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F10_Common', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F10_Common_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F10_Common', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F10_Operating', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F10_Operating_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F10_Operating', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F101', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F101_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F101', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F101_Operating', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F101_Operating_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F101_Operating', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F11', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F11_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F11', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F121', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F121_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F121', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F122', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F122_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F122', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F131', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F131_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F131', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F133', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F133_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F133', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F134', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F134_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F134', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F141', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F141_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F141', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F142', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F142_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F142', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F142_Operating', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F142_Operating_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F142_Operating', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F15', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F15_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F15', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F15_OperatingOnRepair', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F15_OperatingOnRepair_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F15_OperatingOnRepair', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F16_OperationNotes', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F16_OperationNotes_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F16_OperationNotes', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F2_TechData', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F2_TechData_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F2_TechData', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F3_IndividualFeature', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F3_IndividualFeature_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F3_IndividualFeature', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F41', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F41_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F41', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F41a', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F41a_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F41a', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F42', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F42_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F42', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F43', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F43_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F43', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F43_ResourceOperating', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F43_ResourceOperating_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F43_ResourceOperating', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F52', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F52_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F52', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F62', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F62_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F62', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F62_ResourceValues', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F62_ResourceValues_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F62_ResourceValues', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F71', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F71_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F71', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F8', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F8_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F8', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F91', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F91_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F91', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F92', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F92_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F92', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F93', @property = 'article_resolver', @value = 'UPE_MM2_Formular_F93_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'F93', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Formular', @property = 'article_resolver', @value = 'UPE_MM2_Formular_Formular_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Formular', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Ids', @property = 'article_resolver', @value = 'UPE_MM2_Formular_Ids_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Ids', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'IZD_UNIT', @property = 'article_resolver', @value = 'UPE_MM2_Formular_IZD_UNIT_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'IZD_UNIT', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'OperatingAttributes', @property = 'article_resolver', @value = 'UPE_MM2_Formular_OperatingAttributes_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'OperatingAttributes', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'OperatingClassificator', @property = 'article_resolver', @value = 'UPE_MM2_Formular_OperatingClassificator_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'OperatingClassificator', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'OperatingClassificator_Items', @property = 'article_resolver', @value = 'UPE_MM2_Formular_OperatingClassificator_Items_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'OperatingClassificator_Items', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Reports', @property = 'article_resolver', @value = 'UPE_MM2_Formular_Reports_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'Reports', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'UserReadAttentionPoint', @property = 'article_resolver', @value = 'UPE_MM2_Formular_UserReadAttentionPoint_SinventCustomResolver';
EXEC sp_changemergearticle @publication = @publicationVar, @article = 'UserReadAttentionPoint', @property = 'verify_resolver_signature', @value = '0';
EXEC sp_helpmergearticle;
