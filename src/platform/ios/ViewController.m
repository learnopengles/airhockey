#import "ViewController.h"
#include "game.h"

@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.userInteractionEnabled = YES;
    self.preferredFramesPerSecond = 60;
    
    [self setupGL];
}

- (void)dealloc
{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    on_surface_created();
    on_surface_changed([[self view] bounds].size.width, [[self view] bounds].size.height);
}

static CGPoint getNormalizedPoint(UIView* view, CGPoint locationInView)
{
    const float normalizedX = (locationInView.x / view.bounds.size.width) * 2.f - 1.f;
    const float normalizedY = -((locationInView.y / view.bounds.size.height) * 2.f - 1.f);
    return CGPointMake(normalizedX, normalizedY);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    UITouch* touchEvent = [touches anyObject];
    CGPoint locationInView = [touchEvent locationInView:self.view];
    CGPoint normalizedPoint = getNormalizedPoint(self.view, locationInView);
    on_touch_press(normalizedPoint.x, normalizedPoint.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    UITouch* touchEvent = [touches anyObject];
    CGPoint locationInView = [touchEvent locationInView:self.view];
    CGPoint normalizedPoint = getNormalizedPoint(self.view, locationInView);
    on_touch_drag(normalizedPoint.x, normalizedPoint.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled:touches withEvent:event];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    on_draw_frame();
}

@end
