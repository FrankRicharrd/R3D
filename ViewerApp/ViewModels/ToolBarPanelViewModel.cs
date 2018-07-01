using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Commands;
using Prism.Mvvm;
using ViewerCommon;
using Viewer.Interfaces;

namespace ViewerApp.ViewModels
{
    public class ToolBarPanelViewModel :BindableBase
    {
        public DelegateCommand FrontViewCommand { get; set; }

        public DelegateCommand BackViewCommand { get; set; }

        public DelegateCommand RightViewCommand { get; set; }

        public DelegateCommand LeftViewCommand { get; set; }

        public DelegateCommand TopViewCommand { get; set; }

        public DelegateCommand BottomViewCommand { get; set; }

        public DelegateCommand PrespectiveViewCommand { get; set; }

        public DelegateCommand WireframeViewCommand { get; set; }

        public DelegateCommand ShadedViewCommand { get; set; }

        public RelayCommand<Boolean> GridVisibilityCommand { get; set; }

        IApplicationData _data;
        public ToolBarPanelViewModel(IApplicationData data)
        {
            _data = data;
            //FrontViewCommand = new DelegateCommand(FrontViewCommandHandler);

            //BackViewCommand = new DelegateCommand(BackViewCommandHandler);

            //RightViewCommand = new DelegateCommand(RightViewCommandHandler);

            //LeftViewCommand = new DelegateCommand(LeftViewCommandHandler);

            //TopViewCommand = new DelegateCommand(TopViewCommandHandler);

            //BottomViewCommand = new DelegateCommand(BottomViewCommandHandler);

            //PrespectiveViewCommand = new DelegateCommand(PrespectiveViewCommandHandler);

            //WireframeViewCommand = new DelegateCommand(WireframeViewCommandHandler);

            //ShadedViewCommand = new DelegateCommand(ShadedViewCommandHandler);
            //GridVisibilityCommand = new RelayCommand<Boolean>(GridVisibilityCommandHandler);
        }

        //private void GridVisibilityCommandHandler(Boolean obj)
        //{
        //    _editor.SetGridVisibility(obj);
        //}

        //private void ShadedViewCommandHandler()
        //{
        //    _editor.data.Tree.MainParentNode.Inode.SetPolygonMode(1);
        //}

        //private void WireframeViewCommandHandler()
        //{
        //    _editor.data.Tree.MainParentNode.Inode.SetPolygonMode(0);
        //}

        //private void PrespectiveViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Perspective);
        //}

        //private void BottomViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Bottom);
        //}

        //private void TopViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Top);
        //}

        //private void LeftViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Left);
        //}

        //private void RightViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Right);
        //}

        //private void BackViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Back);
        //}

        //private void FrontViewCommandHandler()
        //{
        //    _editor.SetView(FViewType.Front);
        //}
    }
}
