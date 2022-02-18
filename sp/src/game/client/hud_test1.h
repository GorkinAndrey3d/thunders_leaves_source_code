#include "hudelement.h"
#include <vgui_controls\Panel.h>

using namespace vgui;

class CHudImport : public CHudElement, public Panel
{
	DECLARE_CLASS_SIMPLE(CHudImport, Panel);

public:
	CHudImport(const char *pElementName);
	void togglePrint();
	virtual void OnThink();

protected:
	virtual void Paint();
	int m_nImport();
};