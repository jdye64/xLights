#ifndef COLORWASHPANEL_H
#define COLORWASHPANEL_H

//(*Headers(ColorWashPanel)
#include <wx/panel.h>
class wxTextCtrl;
class wxFlexGridSizer;
class wxBitmapButton;
class wxSlider;
class wxStaticText;
class wxCheckBox;
//*)

class ColorWashPanel: public wxPanel
{
	public:

		ColorWashPanel(wxWindow* parent);
		virtual ~ColorWashPanel();

		//(*Declarations(ColorWashPanel)
		wxCheckBox* ShimmerCheckBox;
		wxSlider* SliderCycles;
		wxCheckBox* CircularPaletteCheckBox;
		wxCheckBox* VFadeCheckBox;
		wxBitmapButton* BitmapButton_ColorWashVFade;
		wxCheckBox* HFadeCheckBox;
		wxStaticText* StaticText5;
		wxTextCtrl* CyclesTextCtrl;
		wxBitmapButton* BitmapButton_ColorWashHFade;
		wxBitmapButton* BitmapButton_ColorWashCount;
		//*)


		//(*Identifiers(ColorWashPanel)
		static const long ID_STATICTEXT21;
		static const long IDD_SLIDER_ColorWash_Cycles;
		static const long ID_TEXTCTRL_ColorWash_Cycles;
		static const long ID_BITMAPBUTTON_SLIDER_ColorWash_Count;
		static const long ID_CHECKBOX_ColorWash_VFade;
		static const long ID_BITMAPBUTTON_CHECKBOX_ColorWash_VFade;
		static const long ID_CHECKBOX_ColorWash_HFade;
		static const long ID_BITMAPBUTTON_CHECKBOX_ColorWash_HFade;
		static const long ID_CHECKBOX_ColorWash_Shimmer;
		static const long ID_CHECKBOX_ColorWash_CircularPalette;
		//*)

    
		//(*Handlers(ColorWashPanel)
    void UpdateLinkedSliderFloat(wxCommandEvent& event);
    void UpdateLinkedTextCtrlFloat(wxScrollEvent& event);
    void UpdateLinkedTextCtrl360(wxScrollEvent& event);
    void UpdateLinkedSlider360(wxCommandEvent& event);
    void UpdateLinkedTextCtrl(wxScrollEvent& event);
    void UpdateLinkedSlider(wxCommandEvent& event);
    void OnLockButtonClick(wxCommandEvent& event);
    void UpdateLinkedTextCtrlVC(wxScrollEvent& event);
    void UpdateLinkedTextCtrlFloatVC(wxScrollEvent& event);
    void OnVCButtonClick(wxCommandEvent& event);

		//*)

		DECLARE_EVENT_TABLE()
};

#endif
