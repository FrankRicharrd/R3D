#pragma once
namespace ViewerInterop {

	delegate void NotifyTransformNodeSelected();

	public delegate bool OnTransformNodeSelected(Viewer::Interfaces::IRTransformNode^ node);

	public ref class RSelectionManagerInterop
	{
	public:
		RSelectionManagerInterop(SelectionManager* sel);

		void SubscribeTransformNodeSelected(OnTransformNodeSelected^ callback);

		void NotifyOnTransformNodeSelected();

	private:
		SelectionManager* m_selectionManager;
		OnTransformNodeSelected ^ m_OnTransformNodeSelected;
		NotifyTransformNodeSelected^ m_ontransformNodeselectedNotifier;
	};
}
