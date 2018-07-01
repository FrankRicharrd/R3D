using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Viewer.AdvancedTypes.DockingToolHelper
{
    public abstract class PaneViewModel : ViewModelBase
    {
        public PaneViewModel()
        {
            MinWidth = 250;
            MinHeight = 250;
        }


        string _sourceUri;


        #region Title

        private string _title = null;
        public string Title
        {
            get { return _title; }
            set
            {
                if (_title != value)
                {
                    _title = value;
                    RaisePropertyChanged();
                }
            }
        }

        #endregion

        public ImageSource IconSource
        {
            get;
            protected set;
        }

        #region ContentId

        private string _contentId = null;
        public string ContentId
        {
            get { return _contentId; }
            set
            {
                if (_contentId != value)
                {
                    _contentId = value;
                    RaisePropertyChanged();

                }
            }
        }

        #endregion

        #region IsSelected

        private bool _isSelected = false;
        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                if (_isSelected != value)
                {
                    _isSelected = value;
                    RaisePropertyChanged();

                }
            }
        }

        #endregion

        #region IsActive

        private bool _isActive = false;
        public bool IsActive
        {
            get { return _isActive; }
            set
            {
                if (_isActive != value)
                {
                    _isActive = value;
                    RaisePropertyChanged();
                }
            }
        }


        bool _canHide = true;

        public string SourceUri
        {
            get
            {
                return _sourceUri;
            }

            set
            {
                _sourceUri = value;
            }
        }

        public bool CanHide
        {
            get
            {
                return _canHide;
            }

            set
            {
                _canHide = value;
                RaisePropertyChanged();
            }
        }

        public double MinWidth
        {
            get
            {
                return _minWidth;
            }

            set
            {
                _minWidth = value;
                RaisePropertyChanged();
            }
        }

        public double MinHeight
        {
            get
            {
                return _minHeight;
            }

            set
            {
                _minHeight = value;
                RaisePropertyChanged();
            }
        }

        #endregion


        double _minWidth;
        double _minHeight;

    }
}
