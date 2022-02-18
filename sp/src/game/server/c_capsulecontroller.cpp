#include "cbase.h"

class CCapsuleController : public CLogicalEntity
{
public:
	DECLARE_CLASS(CCapsuleController, CLogicalEntity);
	DECLARE_DATADESC();

	// Конструктор
	CCapsuleController()
	{
		m_capsulecount = 15;
	}

	// Input функция
	void InputSectorNotAvailable(inputdata_t &inputData);
	void InputSectorAvailable(inputdata_t &inputData);
	void InputCallingCapsule(inputdata_t &inputData);

private:

	int	m_nCounter;
	int m_capsulecount;	// Внутренний счётчик
	bool isAvailableSector;
	

	COutputEvent	m_OnThreshold;	// Output событие, когда счётчик достигает порогового значения
	COutputEvent	m_OnReadyFire;	
	COutputEvent	m_OnReadyStateOff;
	COutputEvent	m_OnReadyStateOn;
};

LINK_ENTITY_TO_CLASS(logic_capsule_controller, CCapsuleController);

// Начало описания наших данных для класса
BEGIN_DATADESC(CCapsuleController)

// Для сохранения/загрузки
DEFINE_FIELD(m_nCounter, FIELD_INTEGER),

// Связывает наши переменные с ключевыми значениями из Hammer
DEFINE_KEYFIELD(m_capsulecount, FIELD_INTEGER, "CapsuleCount"),

// Связывает наше input название из Hammer с нашей input функцией

DEFINE_INPUTFUNC(FIELD_VOID, "InputSectorNotAvailable", InputSectorNotAvailable),

DEFINE_INPUTFUNC(FIELD_VOID, "InputSectorAvailable", InputSectorAvailable),

DEFINE_INPUTFUNC(FIELD_VOID, "InputCallingCapsule", InputCallingCapsule),
// Связывает наш ouput с output названием, используемым Hammer
DEFINE_OUTPUT(m_OnThreshold, "OnThreshold"),

DEFINE_OUTPUT(m_OnReadyFire, "OnReadyFire"),
DEFINE_OUTPUT(m_OnReadyStateOff, "OnReadyStateOff"),
DEFINE_OUTPUT(m_OnReadyStateOn, "OnReadyStateOn"),

END_DATADESC()

//-----------------------------------------------------------------------------
// Назначение: Обработка входящих значений (tick input) от других сущностей
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

