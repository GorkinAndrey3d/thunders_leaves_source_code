#include "cbase.h"

class CCapsuleController : public CLogicalEntity
{
public:
	DECLARE_CLASS(CCapsuleController, CLogicalEntity);
	DECLARE_DATADESC();

	// �����������
	CCapsuleController()
	{
		m_capsulecount = 15;
	}

	// Input �������
	void InputSectorNotAvailable(inputdata_t &inputData);
	void InputSectorAvailable(inputdata_t &inputData);
	void InputCallingCapsule(inputdata_t &inputData);

private:

	int	m_nCounter;
	int m_capsulecount;	// ���������� �������
	bool isAvailableSector;
	

	COutputEvent	m_OnThreshold;	// Output �������, ����� ������� ��������� ���������� ��������
	COutputEvent	m_OnReadyFire;	
	COutputEvent	m_OnReadyStateOff;
	COutputEvent	m_OnReadyStateOn;
};

LINK_ENTITY_TO_CLASS(logic_capsule_controller, CCapsuleController);

// ������ �������� ����� ������ ��� ������
BEGIN_DATADESC(CCapsuleController)

// ��� ����������/��������
DEFINE_FIELD(m_nCounter, FIELD_INTEGER),

// ��������� ���� ���������� � ��������� ���������� �� Hammer
DEFINE_KEYFIELD(m_capsulecount, FIELD_INTEGER, "CapsuleCount"),

// ��������� ���� input �������� �� Hammer � ����� input ��������

DEFINE_INPUTFUNC(FIELD_VOID, "InputSectorNotAvailable", InputSectorNotAvailable),

DEFINE_INPUTFUNC(FIELD_VOID, "InputSectorAvailable", InputSectorAvailable),

DEFINE_INPUTFUNC(FIELD_VOID, "InputCallingCapsule", InputCallingCapsule),
// ��������� ��� ouput � output ���������, ������������ Hammer
DEFINE_OUTPUT(m_OnThreshold, "OnThreshold"),

DEFINE_OUTPUT(m_OnReadyFire, "OnReadyFire"),
DEFINE_OUTPUT(m_OnReadyStateOff, "OnReadyStateOff"),
DEFINE_OUTPUT(m_OnReadyStateOn, "OnReadyStateOn"),

END_DATADESC()

//-----------------------------------------------------------------------------
// ����������: ��������� �������� �������� (tick input) �� ������ ���������
//-----------------------------------------------------------------------------



void CCapsuleController::InputSectorNotAvailable(inputdata_t & inputData)
{
	isAvailableSector = 0;
	m_OnReadyStateOff.FireOutput(inputData.pActivator, this);
}

void CCapsuleController::InputSectorAvailable(inputdata_t & inputData)
{
	isAvailableSector = 1;
	m_OnReadyStateOn.FireOutput(inputData.pActivator, this);
}

void CCapsuleController::InputCallingCapsule(inputdata_t & inputData)
{
	if (isAvailableSector == 1 && m_capsulecount > 0)
	{
		m_OnReadyFire.FireOutput(inputData.pActivator, this);
		isAvailableSector = 0;
		m_OnReadyStateOff.FireOutput(inputData.pActivator, this);
	}
}

