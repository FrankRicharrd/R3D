﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Events;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace Viewer.AdvancedTypes.DockingToolHelper
{
    public abstract class ToolViewModel : PaneViewModel, IDockableTool
    {
        public ToolViewModel(string name)
        {
            ContentId = name;
            Name = name;
            Title = name;
        }

        public string Name
        {
            get;
            private set;
        }


        #region IsVisible

        private bool _isVisible = true;
        public bool IsVisible
        {
            get { return _isVisible; }
            set
            {
                if (_isVisible != value)
                {
                    _isVisible = value;
                    RaisePropertyChanged();
                }
            }
        }

        public abstract bool Connect(IApplicationData data, IEventAggregator eventaggre);

        public abstract bool Disconnect();

        #endregion



    }
}
