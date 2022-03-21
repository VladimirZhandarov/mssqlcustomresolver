#ifndef RESOLVEPACKETDATA_H
#define RESOLVEPACKETDATA_H

#include "upelib_global.h"

#include <QtCore/QObject>

#include <QtCore/QVector>
#include <QtCore/QVariant>

#include "memory"

/**
* @class PrimaryNextValue
* ���������� � ��������� ��������� ��������� primary keys
*/
class UPELIB_EXPORT PrimaryNextValue
{
public:
	/**
	* �����������
	* @param fieldName ��� ����
	* @param nextValue ���������� ��������� ��������
	*/
	PrimaryNextValue(const QString& fieldName = "", qint64 nextValue = 0);

	/**
	* ����������
	*/
	~PrimaryNextValue();

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getFieldName() const;

	/**
	* �������� ���������� ��������� ��������
	* @return ���������� ��������� ��������
	*/
	qint64 getNextValue() const;

private:
	QString m_fieldName;	///< ��� ����
	qint64 m_nextValue;		///< ���������� ��������� ��������
};

class FieldData;

/**
* @class PrimaryNextValue
* ���������� � �������� �����
*/
class UPELIB_EXPORT BinaryDataDescription
{
public:
	/**
	* �����������
	* @param fieldName ��� ����
	* @param dataSize ������ �������� ������
	* @param equalData ������� ��� ������ � ���� ���������������� ����� ���������
	* @param nullData ������� =NULL
	*/
	BinaryDataDescription(const QString& fieldName = "", qint64 dataSize = 0, bool equalData = false, bool nullData = false);

	/**
	* ����������
	*/
	~BinaryDataDescription();

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getFieldName() const;

	/**
	* �������� ������ �������� ������
	* @return ������ �������� ������
	*/
	qint64 getDataSize() const;

	/**
	* �������� ������� ��� ������ � ���� ���������������� ����� ���������
	* @return ������� ��� ������ � ���� ���������������� ����� ���������
	*/
	bool isEqualData() const;

	/**
	* �������� ������� =NULL
	* @return ������� =NULL
	*/
	bool isNullData() const;

private:
	QString m_fieldName;	///< ��� ����
	qint64 m_dataSize;		///< ������ �������� ������
	bool m_equalData;		///< ������� ��� ������ � ���� ���������������� ����� ���������
	bool m_nullData;		///< ������� = NULL
};

/**
* @class FieldMetaData
* ���������� ���� ���� ������
*/
class UPELIB_EXPORT FieldMetaData : public QObject
{
	Q_OBJECT
public:
	enum XsdType
	{
		StringXsdType = 0,	///< ������
		IntXsdType			///< �����
	};	///< ��� ����

		/**
		* �����������
		* @param name ��� ����
		* @param type ��� ����
		* @param minOccurs ���������������
		* @param parent ������������ ������
		*/
	FieldMetaData(const QString& name, XsdType type, int minOccurs, QObject* parent = 0);

	/**
	* ����������
	*/
	~FieldMetaData();

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getName() const;

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	XsdType getType() const;

	/**
	* �������� ���������������
	* @return ���������������
	*/
	int getMinOccurs() const;

protected:
	QString m_name;	///< ��� ����
	XsdType m_type;	///< ��� ����
	int m_minOccurs;	///< ���������������
};

/**
* @class ForeignKeyDataSet
* ���������� � ��������� �����
*/
class UPELIB_EXPORT ForeignKeyDataSet
{
public:
	/**
	* �����������
	* @param fieldName ��� ����
	* @param foreignTable �������, ���� ��������� ����
	* @param fieldMetaDataVector ���������� ����� ������, ���� ��������� ����
	* @param dataVector ����, ���� ��������� ����
	* @param binaryDataDescriptions ���������� � �������� �����, ���� ��������� ����
	*/
	ForeignKeyDataSet(const QString& fieldName = "", const QString& foreignTable = "",
		const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector = QVector<std::tr1::shared_ptr<FieldMetaData> >(),
		const QVector<QVariant>& dataVector = QVector<QVariant>(), const QVector<BinaryDataDescription>& binaryDataDescriptions =
		QVector<BinaryDataDescription>());

	/**
	* ����������
	*/
	~ForeignKeyDataSet();

	/**
	* �������� ��� ����
	* @return ��� ����
	*/
	const QString& getFieldName() const;

	/**
	* �������� �������, ���� ��������� ����
	* @return �������, ���� ��������� ����
	*/
	const QString& getForeignTable() const;

	/**
	* �������� ����, ���� ��������� ����
	* @return ����, ���� ��������� ����
	*/
	const QVector<QVariant>& getDataVector() const;

	/**
	* ���������� ��� �������, ���� ��������� ���� (��� GUI)
	* @param tableViewName ��� �������
	*/
	void setTableViewName(const QString& tableViewName);

	/**
	* ���������� ��� ����� ������ �������, ���� ��������� ���� (��� GUI)
	* @param fieldLabelVector ��� ����� �������
	*/
	void setFieldLabelVector(const QVector<std::tr1::shared_ptr<FieldData> >& fieldLabelVector);

	/**
	* �������� ������ ��� tooltip
	* @return ������ ��� tooltip
	*/
	QString getStringValue() const;

	/**
	* �������� �������� rowguid ������ �������, ���� ��������� ����
	* @return �������� rowguid ������ �������, ���� ��������� ����
	*/
	QString getRowGuid() const;

private:
	QString m_fieldName;		///< ��� ����
	QString m_foreignTable;		///< ��� �������, ���� ��������� ����
	const QVector<std::tr1::shared_ptr<FieldMetaData> > m_fieldMetaDataVector;	///< ���������� ����� ������, ���� ��������� ����
	QVector<QVariant> m_dataVector;	///< ����, ���� ��������� ����
	QMap<QString, BinaryDataDescription> m_binaryDataDescriptionMap;	///< ���������� � �������� �����, ���� ��������� ����
	QString m_tableViewName;	///< ��� �������
	QVector<std::tr1::shared_ptr<FieldData> > m_fieldLabelVector; ///< ��� ����� ������ �������, ���� ��������� ����(��� GUI)
};

/**
* @class ResolvePacketData
* ����� ���������� ��������� �������������
*/
class UPELIB_EXPORT ResolvePacketData : public QObject
{
	Q_OBJECT
public:
	enum ResolveType
	{
		UndefineResolveType = -1,
		InsertResolveType = 0,
		InsertErrorResolveType,
		UpdateResolveType,
		UpdateErrorResolveType,
		DeleteResolveType,
		DeleteErrorResolveType,
		UpdateConflictsResolveType,
		UpdateDeleteConflictResolveType
	};	///< ��� ���������

	enum SourceIdentifier
	{
		SourceIsUndefined = -1,
		// ������:
		//     ��������� ������ �������� �� ��������.
		SourceIsPublisher = 0,
		//
		// ������:
		//     ��������� ������ �������� �� ����������.
		SourceIsSubscriber = 1
	};	///< ��� ��������� ���������

	enum ResolveResult
	{
		UndefineResolveResult = 0,
		AcceptResolveResult,
		RejectResolveResult,
		CustomResolveResult,
		Custom2ResolveResult
	};	///< ������� ���������

	/**
	* �����������
	* @param resolveType ��� ���������
	* @param sourceIdentifier ��� ��������� ���������
	* @param publisherName ��� �����������
	* @param subscriberName ��� ����������
	* @param publisherDBName ��� ���� ������ �����������
	* @param subscriberDBName ��� ���� ������ ����������
	* @param tableName ��� �������
	* @param errorCode ��� ������
	* @param errorMessage ��������� ������
	* @param fieldMetaDataVector ���������� ����� ������ �������
	* @param sourceDataVector ���� ������ ��� ������������ �� ����������
	* @param source2DataVector ���� ������ ��� ������������ �� ����������
	* @param result ������� ���������
	* @param parent ������������ ������
	*/
	ResolvePacketData(ResolveType resolveType, SourceIdentifier sourceIdentifier, const QString&  publisherName, const QString&  subscriberName,
		const QString& publisherDBName, const QString& subscriberDBName, const QString& tableName, int errorCode, const QString& errorMessage,
		const QVector<std::tr1::shared_ptr<FieldMetaData> >& fieldMetaDataVector, const QVector<QVariant>& sourceDataVector,
		const QVector<QVariant>& source2DataVector, ResolveResult result, QObject* parent = 0);

	/**
	* ����������
	*/
	~ResolvePacketData();

	/**
	* �������� ��� ���������
	* @return ��� ���������
	*/
	ResolveType getResolveType() const;

	/**
	* �������� ��� ��������� ���������
	* @return ��� ��������� ���������
	*/
	SourceIdentifier getSourceIdentifier() const;

	/**
	* �������� ��� �����������
	* @return ��� �����������
	*/
	const QString& getPublisherName() const;

	/**
	* �������� ��� ����������
	* @return ��� ����������
	*/
	const QString& getSubscriberName() const;

	/**
	* �������� ��� ���� ������ �����������
	* @return
	*/
	const QString& getPublisherDBName() const;

	/**
	* �������� ��� ���� ������ ����������
	* @return ��� ���� ������ ����������
	*/
	const QString& getSubscriberDBName() const;

	/**
	* �������� ��� �������
	* @return ��� �������
	*/
	const QString& getTableName() const;

	/**
	* �������� ��� ������
	* @return ��� ������
	*/
	int getErrorCode() const;

	/**
	* �������� ��������� ������
	* @return ��������� ������
	*/
	const QString& getErrorMessage() const;

	/**
	* �������� ���������� ����� ������ �������
	* @return ���������� ����� ������ �������
	*/
	const QVector<std::tr1::shared_ptr<FieldMetaData> >& getFieldMetaDataVector() const;

	/**
	* �������� ���� ������ ��� ������������ �� ����������
	* @return ���� ������ ��� ������������ �� ����������
	*/
	const QVector<QVariant>& getSourceDataVector() const;

	/**
	* �������� ���� ������ ��� ������������ �� ����������
	* @return ���� ������ ��� ������������ �� ����������
	*/
	const QVector<QVariant>& getSource2DataVector() const;

	/**
	* ���������� ���������� � ��������� ��������� ��������� primary keys ��� ����������
	* @param sourcePrimaryNextValues ��������� ��������� �������� primary keys ��� ����������
	*/
	void setSourcePrimaryNextValues(const QVector<PrimaryNextValue>& sourcePrimaryNextValues);

	/**
	* �������� ���������� � ��������� ��������� ��������� primary keys ��� ����������
	* @return ��������� ��������� �������� primary keys ��� ����������
	*/
	const QVector<PrimaryNextValue>& getSourcePrimaryNextValues() const;

	/**
	* ���������� ���������� � ��������� ��������� ��������� primary keys ��� ����������
	* @param source2PrimaryNextValues ��������� ��������� �������� primary keys ��� ����������
	*/
	void setSource2PrimaryNextValues(const QVector<PrimaryNextValue>& source2PrimaryNextValues);

	/**
	* �������� ���������� � ��������� ��������� ��������� primary keys ��� ����������
	* @return ��������� ��������� �������� primary keys ��� ����������
	*/
	const QVector<PrimaryNextValue>& getSource2PrimaryNextValues() const;

	/**
	* ���������� ���������� � ��������� ������ ��� ����������
	* @param sourceDataSetForeignKeys ���������� � ��������� ������ ��� ����������
	*/
	void setSourceDataSetForeignKeys(const QVector<ForeignKeyDataSet>& sourceDataSetForeignKeys);

	/**
	* �������� ���������� � ��������� ������ ��� ����������
	* @return ���������� � ��������� ������ ��� ����������
	*/
	const QVector<ForeignKeyDataSet>& getSourceDataSetForeignKeys() const;

	/**
	* ���������� ���������� � ��������� ������ ��� ����������
	* @param source2DataSetForeignKeys ���������� � ��������� ������ ��� ����������
	*/
	void setSource2DataSetForeignKeys(const QVector<ForeignKeyDataSet>& source2DataSetForeignKeys);

	/**
	* �������� ���������� � ��������� ������ ��� ����������
	* @return ���������� � ��������� ������ ��� ����������
	*/
	const QVector<ForeignKeyDataSet>& getSource2DataSetForeignKeys() const;

	/**
	* ���������� ���������� � �������� ����� ��� ����������
	* @param sourceBinaryDataDescriptions ���������� � �������� ����� ��� ����������
	*/
	void setSourceBinaryDataDescriptions(const QVector<BinaryDataDescription>& sourceBinaryDataDescriptions);

	/**
	* �������� ���������� � �������� ����� ��� ����������
	* @return ���������� � �������� ����� ��� ����������
	*/
	BinaryDataDescription getSourceBinaryDataDescription(const QString& name) const;

	/**
	* ���������� ���������� � �������� ����� ��� ����������
	* @param source2BinaryDataDescriptions ���������� � �������� ����� ��� ����������
	*/
	void setSource2BinaryDataDescriptions(const QVector<BinaryDataDescription>& source2BinaryDataDescriptions);

	/**
	* �������� ���������� � �������� ����� ��� ����������
	* @return ���������� � �������� ����� ��� ����������
	*/
	BinaryDataDescription getSource2BinaryDataDescription(const QString& name) const;

	/**
	* �������� ������������ �������� ��� ���������� ����� �� ����� ����
	* @param fieldName ��� ���� ��� ���������� �����
	* @return ������������ �������� ��� ���������� �����
	*/
	qint64 getMaxSourcesPrimaryNextValue(const QString& fieldName);

	/**
	* �������� ������� ���������
	* @return ������� ���������
	*/
	ResolveResult getResult() const;

	/**
	* �������� ������ tooltip ��� ���������� ����� �� ����� ���� ��� ����������
	* @param fieldName ��� ���� ��� ���������� �����
	* @return ������ tooltip
	*/
	QString getSourceToolTipString(const QString& fieldName) const;

	/**
	* �������� ������ tooltip ��� ���������� ����� �� ����� ���� ��� ����������
	* @param fieldName ��� ���� ��� ���������� �����
	* @return ������ tooltip
	*/
	QString getSource2ToolTipString(const QString& fieldName) const;

protected:
	ResolveType m_resolveType;				///< ��� ���������
	SourceIdentifier m_sourceIdentifier;	///< ��������� ���������
	QString m_publisherName;	///< ��� �����������
	QString m_subscriberName;	///< ��� ����������
	QString m_publisherDBName;	///< ��� ���� ������ �����������
	QString m_subscriberDBName;	///< ��� ���� ������ ����������
	QString m_tableName;		///< ��� �������
	int m_errorCode;			///< ��� ������
	QString m_errorMessage;		///< ��������� ������
	QVector<std::tr1::shared_ptr<FieldMetaData> > m_fieldMetaDataVector;	///< ���������� ����� ������ �������
	QVector<QVariant> m_sourceDataVector;	///< ���� ������ ��� ������������ �� ����������
	QVector<QVariant> m_source2DataVector;	///< ���� ������ ��� ������������ �� ����������
	QVector<PrimaryNextValue> m_sourcePrimaryNextValues;	///< ���������� � ��������� ��������� ��������� primary keys ��� ����������
	QVector<PrimaryNextValue> m_source2PrimaryNextValues;	///< ���������� � ��������� ��������� ��������� primary keys ��� ����������
	QVector<ForeignKeyDataSet> m_sourceDataSetForeignKeys;	///< ���������� � ��������� ������ ��� ����������
	QVector<ForeignKeyDataSet> m_source2DataSetForeignKeys;	///< ���������� � ��������� ������ ��� ����������
	QMap<QString, BinaryDataDescription> m_sourceBinaryDataDescriptionMap;	///< ���������� � �������� ����� ��� ����������
	QMap<QString, BinaryDataDescription> m_source2BinaryDataDescriptionMap;	///< ���������� � �������� ����� ��� ����������

	ResolveResult m_result;		///< ������� ���������
};

#endif // RESOLVEPACKETDATA_H
