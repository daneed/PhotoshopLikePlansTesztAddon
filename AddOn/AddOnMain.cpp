#include "APIEnvir.h"
#include "ACAPinc.h"

#include "ResourceIds.hpp"
#include "DGModule.hpp"

#include "HatchRelatedAPIData.hpp"

static const GSResID AddOnInfoID			= ID_ADDON_INFO;
	static const Int32 AddOnNameID			= 1;
	static const Int32 AddOnDescriptionID	= 2;
	static const Int32 FountFillCountFormatStrId = 3;

static const short AddOnMenuID				= ID_ADDON_MENU;
	static const Int32 AddOnCommandID		= 1;

class ExampleDialog :	public DG::ModalDialog,
						public DG::PanelObserver,
						public DG::ButtonItemObserver,
						public DG::CompoundItemObserver
{
public:
	enum DialogResourceIds
	{
		ExampleDialogResourceId = ID_ADDON_DLG,
		OKButtonId				= 1,
		CancelButtonId			= 2,
		SeparatorId				= 3,
		ListBoxId				= 4,
		FillCountTextId			= 5
	};

	ExampleDialog () :
		DG::ModalDialog (ACAPI_GetOwnResModule (), ExampleDialogResourceId, ACAPI_GetOwnResModule ()),
		okButton (GetReference (), OKButtonId),
		cancelButton (GetReference (), CancelButtonId),
		separator (GetReference (), SeparatorId),
		hatchListBox (GetReference (), ListBoxId),
		fillCountText (GetReference (), FillCountTextId)
	{
		AttachToAllItems (*this);
		Attach (*this);
	}

	~ExampleDialog ()
	{
		Detach (*this);
		DetachFromAllItems (*this);
	}

private:
	virtual void	PanelOpened (const DG::PanelOpenEvent& /*ev*/)
	{
		hatchListBox.SetHeaderSynchronState (true);
		hatchListBox.SetHeaderItemCount (8);
		hatchListBox.SetTabFieldCount (8);

		hatchListBox.SetHeaderItemSizeableFlag (1, false);
		hatchListBox.SetHeaderItemSizeableFlag (2, false);
		hatchListBox.SetHeaderItemSizeableFlag (3, false);
		hatchListBox.SetHeaderItemSizeableFlag (4, false);
		hatchListBox.SetHeaderItemSizeableFlag (5, false);
		hatchListBox.SetHeaderItemSizeableFlag (6, false);
		hatchListBox.SetHeaderItemSizeableFlag (7, false);
		hatchListBox.SetHeaderItemSizeableFlag (8, false);

		hatchListBox.SetHeaderItemStyle (1, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (2, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (3, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (4, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (5, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (6, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (7, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemStyle (8, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetHeaderItemText (1, "IND");
		hatchListBox.SetHeaderItemText (2, "SubType");
		hatchListBox.SetHeaderItemText (3, "FillInd");
		hatchListBox.SetHeaderItemText (4, "BMatInd");
		hatchListBox.SetHeaderItemText (5, "FgPen");
		hatchListBox.SetHeaderItemText (6, "FGColor");
		hatchListBox.SetHeaderItemText (7, "BGPen");
		hatchListBox.SetHeaderItemText (8, "BGColor");
		ReLayoutListBox (0);

		hatchListBox.SetTabFieldCount (8);

		GS::Array<API_HatchType> hatchTypeList = PhotoshopLikePlans::GetHatchData ();

		fillCountText.SetText (GS::UniString::SPrintf (RSGetIndString (AddOnInfoID, FountFillCountFormatStrId, ACAPI_GetOwnResModule ()), hatchTypeList.GetSize ()));

		FillListBox (hatchTypeList);
	}

	virtual void PanelResized (const DG::PanelResizeEvent& ev) override
	{
		BeginMoveResizeItems ();
		okButton.Move (ev.GetHorizontalChange (), ev.GetVerticalChange ());
		cancelButton.Move (ev.GetHorizontalChange (), ev.GetVerticalChange ());
		separator.MoveAndResize (0, ev.GetVerticalChange (), ev.GetHorizontalChange (), 0);
		hatchListBox.MoveAndResize (0, 0, ev.GetHorizontalChange (), ev.GetVerticalChange ());

		fillCountText.Move (ev.GetHorizontalChange (), ev.GetVerticalChange ());
		EndMoveResizeItems ();

		ReLayoutListBox (ev.GetHorizontalChange ());
	}

	virtual void ButtonClicked (const DG::ButtonClickEvent& ev) override
	{
		if (ev.GetSource () == &okButton) {
			PostCloseRequest (DG::ModalDialog::Accept);
		} else if (ev.GetSource () == &cancelButton) {
			PostCloseRequest (DG::ModalDialog::Cancel);
		}
	}


	void ReLayoutListBox (short /*horizontalChange*/)
	{
		hatchListBox.SetTabFieldProperties (1, 0, 49, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (2, 50, 99, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (3, 100, 149, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (4, 150, 199, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (5, 200, 249, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (6, 250, 349, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (7, 350, 399, DG::ListBox::Left, DG::ListBox::EndTruncate);
		hatchListBox.SetTabFieldProperties (8, 400, 499, DG::ListBox::Left, DG::ListBox::EndTruncate);
	}

	void FillListBox (const GS::Array<API_HatchType>& hatchTypeList)
	{
		hatchListBox.DeleteItem (DG::ListBox::AllItems);

		Int32 index = 0;
		for (auto& hatchType : hatchTypeList) {
			hatchListBox.AppendItem ();

			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 1, GS::UniString::SPrintf ("%d", index++));
			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 2, hatchType.hatchType == API_FillHatch ? "Fill" : "BMat.");
			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 3, GS::UniString::SPrintf ("%d", hatchType.fillInd));
			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 4, GS::UniString::SPrintf ("%d", hatchType.buildingMaterial));

			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 5, GS::UniString::SPrintf ("%d", hatchType.fillPen.penIndex));
			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 6, ToString (hatchType.foregroundRGB));
			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 7, GS::UniString::SPrintf ("%d", hatchType.fillBGPen));
			hatchListBox.SetTabItemText (hatchListBox.GetItemCount (), 8, ToString (hatchType.backgroundRGB));
		}
	}

	GS::UniString ToString (const API_RGBColor& color) {
		return GS::UniString::SPrintf ("[%0.0f,%0.0f,%0.0f]", color.f_red * 255, color.f_green * 255, color.f_blue * 255);
	}

	DG::Button				okButton;
	DG::Button				cancelButton;
	DG::Separator			separator;
	DG::SingleSelListBox	hatchListBox;
	DG::LeftText			fillCountText;
};

static GSErrCode MenuCommandHandler (const API_MenuParams *menuParams)
{
	switch (menuParams->menuItemRef.menuResID) {
		case AddOnMenuID:
			switch (menuParams->menuItemRef.itemIndex) {
				case AddOnCommandID:
					{
						ExampleDialog dialog;
						dialog.Invoke ();
					}
					break;
			}
			break;
	}
	return NoError;
}

API_AddonType __ACDLL_CALL CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name, AddOnInfoID, AddOnNameID, ACAPI_GetOwnResModule ());
	RSGetIndString (&envir->addOnInfo.description, AddOnInfoID, AddOnDescriptionID, ACAPI_GetOwnResModule ());

	return APIAddon_Normal;
}

GSErrCode __ACDLL_CALL RegisterInterface (void)
{
	return ACAPI_Register_Menu (AddOnMenuID, 0, MenuCode_Tools, MenuFlag_Default);
}

GSErrCode __ACENV_CALL Initialize (void)
{
	return ACAPI_Install_MenuHandler (AddOnMenuID, MenuCommandHandler);
}

GSErrCode __ACENV_CALL FreeData (void)
{
	return NoError;
}
