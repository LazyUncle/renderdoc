import renderdoc as rd
import rdtest
from PIL import Image


class VK_Sample_Locations(rdtest.TestCase):
    def get_capture(self):
        return rdtest.run_and_capture("demos_x64", "VK_Sample_Locations", 5)

    def check_capture(self):
        draw: rd.DrawcallDescription = self.find_draw("Degenerate")
        self.controller.SetFrameEvent(draw.next.eventId, True)
        pipe: rd.VKState = self.controller.GetVulkanPipelineState()

        if pipe.multisample.rasterSamples != 4:
            raise rdtest.TestFailureException("MSAA sample count is {}, not 1".format(pipe.multisample.rasterSamples))

        sampleLoc: rd.VKSampleLocations = pipe.multisample.sampleLocations

        if sampleLoc.gridWidth != 1:
            raise rdtest.TestFailureException("Sample locations grid width is {}, not 1".format(sampleLoc.gridWidth))
        if sampleLoc.gridHeight != 1:
            raise rdtest.TestFailureException("Sample locations grid height is {}, not 1".format(sampleLoc.gridHeight))

        # [0] and [1] should be identical, as should [2] and [3], but they should be different from each other
        if not sampleLoc.customLocations[0] == sampleLoc.customLocations[1]:
            raise rdtest.TestFailureException("In degenerate case, sample locations [0] and [1] don't match: {} vs {}"
                                              .format(sampleLoc.customLocations[0], sampleLoc.customLocations[1]))

        if not sampleLoc.customLocations[2] == sampleLoc.customLocations[3]:
            raise rdtest.TestFailureException("In degenerate case, sample locations [2] and [3] don't match: {} vs {}"
                                              .format(sampleLoc.customLocations[2], sampleLoc.customLocations[3]))

        if sampleLoc.customLocations[1] == sampleLoc.customLocations[2]:
            raise rdtest.TestFailureException("In degenerate case, sample locations [1] and [2] DO match: {} vs {}"
                                              .format(sampleLoc.customLocations[1], sampleLoc.customLocations[2]))

        draw: rd.DrawcallDescription = self.find_draw("Rotated")
        self.controller.SetFrameEvent(draw.next.eventId, True)
        pipe: rd.VKState = self.controller.GetVulkanPipelineState()

        if pipe.multisample.rasterSamples != 4:
            raise rdtest.TestFailureException("MSAA sample count is {}, not 1".format(pipe.multisample.rasterSamples))

        sampleLoc: rd.VKSampleLocations = pipe.multisample.sampleLocations

        if sampleLoc.gridWidth != 1:
            raise rdtest.TestFailureException("Sample locations grid width is {}, not 1".format(sampleLoc.gridWidth))
        if sampleLoc.gridHeight != 1:
            raise rdtest.TestFailureException("Sample locations grid height is {}, not 1".format(sampleLoc.gridHeight))

        # All sample locations should be unique
        if sampleLoc.customLocations[0] == sampleLoc.customLocations[1]:
            raise rdtest.TestFailureException("In rotated case, sample locations [0] and [1] DO match: {} vs {}"
                                              .format(sampleLoc.customLocations[0], sampleLoc.customLocations[1]))

        if sampleLoc.customLocations[1] == sampleLoc.customLocations[2]:
            raise rdtest.TestFailureException("In rotated case, sample locations [1] and [2] DO match: {} vs {}"
                                              .format(sampleLoc.customLocations[1], sampleLoc.customLocations[2]))

        if sampleLoc.customLocations[2] == sampleLoc.customLocations[3]:
            raise rdtest.TestFailureException("In rotated case, sample locations [2] and [3] DO match: {} vs {}"
                                              .format(sampleLoc.customLocations[2], sampleLoc.customLocations[3]))

        rdtest.log.success("Pipeline state is correct")

        # Grab the multisampled image's ID here
        save_data = rd.TextureSave()
        curpass: rd.VKCurrentPass = pipe.currentPass
        save_data.resourceId = curpass.framebuffer.attachments[curpass.renderpass.colorAttachments[0]].imageResourceId
        save_data.destType = rd.FileType.PNG
        save_data.sample.mapToArray = False

        dim = (0, 0)
        texs = self.controller.GetTextures()
        for tex in texs:
            if tex.resourceId == save_data.resourceId:
                dim = (tex.width, tex.height)

        if dim == (0,0):
            raise rdtest.TestFailureException("Couldn't get dimensions of texture")

        last_draw: rd.DrawcallDescription = self.get_last_draw()

        self.controller.SetFrameEvent(last_draw.eventId, True)

        # Due to the variability of rasterization between implementations or even drivers,
        # we don't want to check against a 'known good'.
        # So instead we verify that at the first degenerate draw each pair of two sample's images are identical and that
        # in the rotated grid case each sample's image is distinct.
        # In future we could also check that the degenerate case 'stretches' the triangle up, as with the way the
        # geometry is defined the second sample image should be a superset (i.e. strictly more samples covered).
        rotated_paths = []
        degenerate_paths = []

        for sample in range(0, 4):
            tmp_path = rdtest.get_tmp_path('sample{}.png'.format(sample))
            degenerate_path = rdtest.get_tmp_path('degenerate{}.png'.format(sample))
            rotated_path = rdtest.get_tmp_path('rotated{}.png'.format(sample))

            rotated_paths.append(rotated_path)
            degenerate_paths.append(degenerate_path)

            save_data.sample.sampleIndex = sample
            self.controller.SaveTexture(save_data, tmp_path)

            try:
                img = Image.open(tmp_path)
            except Exception as ex:
                raise FileNotFoundError("Can't open {}".format(rdtest.sanitise_filename(tmp_path)))

            img.crop((0, 0, dim[0]/2, dim[1])).save(degenerate_path)
            img.crop((dim[0]/2, 0, dim[0], dim[1])).save(rotated_path)

        # first two degenerate images should be identical, as should the last two, and they should be different.
        if not rdtest.image_compare(degenerate_paths[0], degenerate_paths[1], 0):
            raise rdtest.TestFailureException("Degenerate grid sample 0 and 1 are different",
                                              degenerate_paths[0], degenerate_paths[1])

        if not rdtest.image_compare(degenerate_paths[2], degenerate_paths[3], 0):
            raise rdtest.TestFailureException("Degenerate grid sample 2 and 3 are different",
                                              degenerate_paths[2], degenerate_paths[3])

        if rdtest.image_compare(degenerate_paths[1], degenerate_paths[2], 0):
            raise rdtest.TestFailureException("Degenerate grid sample 1 and 2 are identical",
                                              degenerate_paths[1], degenerate_paths[2])

        rdtest.log.success("Degenerate grid sample images are as expected")

        # all rotated images should be different
        for A in range(0, 4):
            for B in range(A+1, 4):
                if rdtest.image_compare(rotated_paths[A], rotated_paths[B], 0):
                    raise rdtest.TestFailureException("Rotated grid sample {} and {} are identical".format(A, B),
                                                      rotated_paths[A], rotated_paths[B])

        rdtest.log.success("Rotated grid sample images are as expected")
