//
// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: Apache-2.0
//
using UIKit;

namespace OneDsCppSdk.iOS.Bindings.Tests
{
    public class Application
    {
        // This is the main entry point of the application.
        static void Main(string[] args)
        {
            // if you want to use a different Application Delegate class from "UnitTestAppDelegate"
            // you can specify it here.
            UIApplication.Main(args, null, "UnitTestAppDelegate");
        }
    }
}
