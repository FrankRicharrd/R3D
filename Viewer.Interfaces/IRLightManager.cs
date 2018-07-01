using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Viewer.Interfaces
{
    public interface IRLightManager
    {
        void SwitchMainLight(bool enable);
        void HeadLightSwitch(bool v);
        void SkyLightSwitch(bool v);
        void ExtraLights(bool v);

        IRLight CreateLight(IRTransformNode transNode, Color color, double posX, double posY, double posZ);
    }
}
