using Prism.Mvvm;
using System.Collections.ObjectModel;
using Prism.Events;
using Viewer.Infrastructure;
using Viewer.Interfaces;
using Viewer.Tools.ViewModels;

namespace ViewerApp.ViewModels
{
    //public class TreePanelViewModel : BindableBase
    //{
    //    ReadOnlyCollection<UI_RNode> _children;
    //    IApplicationData _data;
    //    IEventAggregator _eventAggregator;
    //    public TreePanelViewModel(IApplicationData data, IEventAggregator eventAggregator)
    //    {
    //        _data = data;
    //        _eventAggregator = eventAggregator;
            

    //        _eventAggregator.GetEvent<OnContextCreatedEvent>().Subscribe(OnContextCreatedHandler);

    //    }

    //    private void OnContextCreatedHandler(IRNode obj)
    //    {
    //        //List<UI_RNode> lst = new List<UI_RNode>();
    //        //lst.Add(new UI_RTransformNode(_data.ActiveDocument.Tree.GeometryParentNode, null));

    //        //_children = new ReadOnlyCollection<UI_RNode>(lst);
    //        //OnPropertyChanged("Children");
    //    }

    //    public void TreeViewSelectionChanged(object data)
    //    {
    //        if (data is IRNode)
    //        {
    //            _eventAggregator.GetEvent<NodeSelectionChangedEvent>().Publish(data as IRNode);
    //        }
    //    }


    //    public ReadOnlyCollection<UI_RNode> Children
    //    {
    //        get
    //        {
    //            return _children;
    //        }
    //    }

      
    //}
}
