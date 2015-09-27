using System;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;

namespace MediaFoundation.Tests
{
    [TestClass]
    public class DurationTests
    {
        [TestMethod]
        public async Task GetDurationFromUrl()
        {
            using (var sr = await SourceReader.CreateFromUriAsync(
                new Uri("http://www.giantbomb.com/podcasts/download/1313/Giant_Bombcast_08_11_2015-08-11-2015-6792778640.mp3")))
            {
                var duration = sr.GetDurationBlocking();

                var seconds = (int)Math.Round(duration.TotalSeconds);

                Assert.AreEqual(10718, seconds);
            }
        }
    }
}