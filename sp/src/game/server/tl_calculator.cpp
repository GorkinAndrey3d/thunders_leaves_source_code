#include "cbase.h"

class CTLCalculator :public CLogicalEntity
{
public:
	DECLARE_CLASS(CTLCalculator, CLogicalEntity);
	DECLARE_DATADESC();
	CTLCalculator()
	{
		m_nCounter = 0;
	}
	void InputTick(inputdata_t &inputData);
private:
	int m_nThreshold;
	int m_nCounter;
	COutputEvent m_nOnThreshold;
};

LINK_ENTITY_TO_CLASS(tl_calculator, CTLCalculator);

BEGIN_DATADESC(CTLCalculator)

DEFINE_FIELD(m_nCounter, FIELD_INTEGER),
DEFINE_KEYFIELD(m_nThreshold, FIELD_INTEGER, "threshold"),
DEFINE_INPUTFUNC(FIELD_VOID, "Tick", InputTick),
DEFINE_OUTPUT(m_nOnThreshold, "OnThreshold"),

END_DATADESC()

void CTLCalculator::InputTick(inputdata_t &inputData)
{
	m_nCounter++;
	if (m_nCounter >= m_nThreshold)
	{
		m_nOnThreshold.FireOutput(inputData.pActivator, this);
		m_nCounter = 0;
	}
}