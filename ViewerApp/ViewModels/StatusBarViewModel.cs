using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Mvvm;
using Prism.Events;
using Viewer.Infrastructure;

namespace ViewerApp.ViewModels
{
    public class StatusBarViewModel : BindableBase
    {

        IEventAggregator _eventAggregator;
        string _statusContent;
        public StatusBarViewModel(IEventAggregator eventAggregator)
        {
            _eventAggregator = eventAggregator;

            _eventAggregator.GetEvent<StatusBarMessageEvent>().Subscribe(OnContextCreatedHandler);
        }

        public string StatusContent
        {
            get
            {
                return _statusContent;
            }

            set
            {
                _statusContent = value;
                OnPropertyChanged();
            }
        }

        private void OnContextCreatedHandler(string obj)
        {
            StatusContent = obj;
        }
    }
}
